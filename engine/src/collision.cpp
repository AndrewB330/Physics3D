#include "../include/engine/collision.hpp"
#include <queue>
#include <array>
#include <map>

CollisionDetector::CollisionDetector(Object* a, Object* b) :a(a), b(b) {
    if (IsIntersect(a->GetBBox(), b->GetBBox()) && GilbertJohnsonKeerthi()) {
        collided = true;
        ExpandingPolytopeAlgorithm();
        GenerateCollisions();
    }
}

const std::vector<Collision>& CollisionDetector::GetCollisions() const {
    return collisions;
}

bool CollisionDetector::HasCollision() const {
    return collided; // todo: change to !empty()
}

bool CollisionDetector::GilbertJohnsonKeerthi() {
    vertices.clear();
    Vec3 b0 = Vec3(1, 0, 0);
    Vec3 b1 = Vec3(-0.5, 0, sqrt(3) / 2);
    Vec3 b2 = Vec3(-0.5, 0, -sqrt(3) / 2);
    for (int iter = 0; iter < 100 && vertices.empty(); iter++) {
        Mat3 rot = Quat::RandomRotation().Mat();
        Vec3 s0 = GetSupportingVector(rot * b0);
        Vec3 s1 = GetSupportingVector(rot * b1);
        Vec3 s2 = GetSupportingVector(rot * b2);
        double volume = Dot(s0, Cross(s1, s2));
        if (abs(volume) > 1e-6) {
            vertices.push_back(s0);
            vertices.push_back(s1);
            vertices.push_back(s2);
            Vec3 normal = Cross(s1 - s0, s2 - s0);
            if (Dot(normal, -s0) < 0) {
                normal *= -1;
                std::swap(vertices[0], vertices[1]);
            }
            vertices.push_back(GetSupportingVector(normal));
        }
    }

    if (vertices.empty()) {
        return false;
    }

    for (int iter = 0; iter < 300; iter++) {
        Vec3 da = vertices[3] - vertices[0];
        Vec3 db = vertices[3] - vertices[1];
        Vec3 dc = vertices[3] - vertices[2];

        Vec3 d0 = -vertices[3];

        Vec3 abdNorm = Cross(da, db);
        Vec3 bcdNorm = Cross(db, dc);
        Vec3 cadNorm = Cross(dc, da);

        Vec3 normal = Vec3();

        if (Dot(abdNorm, d0) > 0) {
            vertices.erase(vertices.begin() + 2);
            normal = abdNorm;
        } else if (Dot(bcdNorm, d0) > 0) {
            vertices.erase(vertices.begin());
            normal = bcdNorm;
        } else if (Dot(cadNorm, d0) > 0) {
            vertices.erase(vertices.begin() + 1);
            std::swap(vertices[1], vertices[2]);
            normal = cadNorm;
        } else {
            return true;
        }


        Vec3 supporting_vector = GetSupportingVector(normal);
        vertices.push_back(supporting_vector);
        if (Dot(supporting_vector, normal) < 0)
            return false;
    }
    return false;
}

void CollisionDetector::ExpandingPolytopeAlgorithm() {
    std::vector<std::array<int, 3>> vert_id(4);
    std::vector<std::array<int, 3>> adj_id(4);
    vert_id[0] = { 0, 1, 3 };
    vert_id[1] = { 1, 2, 3 };
    vert_id[2] = { 2, 0, 3 };
    vert_id[3] = { 1, 0, 2 };
    adj_id[0] = { 1, 2, 3 };
    adj_id[1] = { 2, 0, 3 };
    adj_id[2] = { 0, 1, 3 };
    adj_id[3] = { 2, 1, 0 };

    std::vector<int> face_deleted(4, 0);
    std::queue<int> face_pool;

    for (int iter = 0; iter < 200; iter++) {
        int nearest_face = -1;
        Vec3 nearest_face_normal;
        double nearest_face_distance = std::numeric_limits<double>::max();
        for (int face = 0; face < vertices.size(); face++) {
            if (face_deleted[face]) {
                continue;
            }
            Vec3 v0 = vertices[vert_id[face][0]];
            Vec3 v1 = vertices[vert_id[face][1]];
            Vec3 v2 = vertices[vert_id[face][2]];
            Vec3 norm = Cross(v1 - v0, v2 - v0).Norm();
            double distance = Dot(v0, norm);
            if (distance < nearest_face_distance) {
                nearest_face = face;
                nearest_face_distance = distance;
                nearest_face_normal = norm;
            }
        }
        if (nearest_face == -1) {
            return; // should not happen add ASSERT
        }
        Vec3 supporting_vertex = GetSupportingVector(nearest_face_normal);
        if (abs(Dot(supporting_vertex, nearest_face_normal) - nearest_face_distance) < 1e-6) {
            return;
        }

        std::queue<int> q;
        q.push(nearest_face);

        std::vector<bool> face_visited(vert_id.size());

        std::vector<std::tuple<int, int, int, int>> ridges;
        std::vector<int> faces_to_delete;
        while (!q.empty()) {
            int cur_face = q.front();
            q.pop();
            faces_to_delete.push_back(cur_face);
            for (int side = 0; side < 3; side++) {
                int adj_face = adj_id[cur_face][side];
                Vec3 v0 = vertices[vert_id[adj_face][0]];
                Vec3 v1 = vertices[vert_id[adj_face][1]];
                Vec3 v2 = vertices[vert_id[adj_face][2]];
                double volume = Dot(v0 - supporting_vertex, Cross(v1 - supporting_vertex, v2 - supporting_vertex));
                if (volume < 0) {
                    int opposite_face = adj_id[adj_face][side];
                    int opposite_side = 0;
                    for (; opposite_side < 3 && adj_id[opposite_face][opposite_side] != adj_face; opposite_side++) {}
                    if (opposite_side == 3) {
                        // should not happen
                    }
                    ridges.push_back({ adj_face, side, opposite_face, opposite_side });
                } else if (!face_visited[adj_face]) {
                    face_visited[adj_face] = true;
                    q.push(adj_face);
                }
            }
        }
        std::vector<int> new_face_ids;
        for (auto [old_face_id, old_side, face_id, side] : ridges) {
            int new_face_id = -1;
            if (face_pool.empty()) {
                new_face_id = vert_id.size();
                vert_id.push_back({});
                adj_id.push_back({});
            } else {
                new_face_id = face_pool.front();
                face_pool.pop();
            }
            auto& new_adj_id = adj_id[new_face_id];
            auto& new_vert_id = vert_id[new_face_id];

            new_vert_id = vert_id[old_face_id];
            new_vert_id[old_side] = vertices.size();

            new_adj_id[old_side] = face_id;
            adj_id[face_id][side] = new_face_id;

            new_face_ids.push_back(new_face_id);
        }
        std::map<std::pair<int, int>, std::pair<int,int>> ridge_map;
        for (int face_id : new_face_ids) {
            for (int side = 0; side < 3; side++) {
                std::pair<int, int> ridge = { vert_id[face_id][(side + 1) % 3], vert_id[face_id][(side + 2) % 3] };
                if (ridge.first > ridge.second) {
                    std::swap(ridge.first, ridge.second);
                }
                if (ridge_map.find(ridge) != ridge_map.end()) {
                    auto [other_face, other_side] = ridge_map[ridge];
                    adj_id[other_face][other_side] = face_id;
                    adj_id[face_id][side] = other_face;
                } else {
                    ridge_map[ridge] = { face_id, side };
                }
            }
        }

        for (int face : faces_to_delete) {
            face_pool.push(face);
            face_deleted[face] = true;
        }

        vertices.push_back(supporting_vertex);
    }
}

void CollisionDetector::GenerateCollisions() {}

Vec3 CollisionDetector::GetSupportingVector(Vec3 dir) {
    return a->GetSupportingVector(dir) - b->GetSupportingVector(-dir);
}
