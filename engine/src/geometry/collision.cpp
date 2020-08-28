#include <engine/geometry/collision.hpp>
#include <engine/assert.hpp>
#include <queue>
#include <array>
#include <map>
#include <set>

CollisionDetector::CollisionDetector(const ConvexShape *a, const ConvexShape *b) {
    if (!IsIntersect(a->GetBBox(), b->GetBBox())) {
        return;
    }
    GilbertJohnsonKeerthi gjk(a, b);
    if (!gjk.ContainsOrigin()) {
        return;
    }
    ExpandingPolytopeAlgorithm epa(a, b, gjk.GetSimplex());
    ENGINE_ASSERT(epa.AnswerFound(), "EPA Answer not found");
    if (!epa.AnswerFound()) {
        return;
    }
    if (auto s = dynamic_cast<const SphereShape *>(a)) {
        collisions.push_back(Collision{
                s->GetTranslation() + epa.GetDirection() * (s->GetRadius() - epa.GetDistance() * 0.5),
                epa.GetDirection(),
                epa.GetDistance()
        });
        return;
    }
    if (auto s = dynamic_cast<const SphereShape *>(b)) {
        collisions.push_back(Collision{
                s->GetTranslation() - epa.GetDirection() * (s->GetRadius() - epa.GetDistance() * 0.5),
                epa.GetDirection(),
                epa.GetDistance()
        });
        return;
    }
    auto p1 = dynamic_cast<const ConvexPolyhedronShape *>(a);
    auto p2 = dynamic_cast<const ConvexPolyhedronShape *>(b);

    double projection_max = Dot(epa.GetDirection(), p1->GetSupportingVector(epa.GetDirection()));
    double projection_min = Dot(epa.GetDirection(), p2->GetSupportingVector(-epa.GetDirection()));

    ENGINE_ASSERT(projection_min <= projection_max, "Collision detection error");

    for (const Vec3 &v : p1->GetVertices()) {
        if (Dot(v, epa.GetDirection()) >= projection_min && p2->IsPointInside(v)) {
            collisions.push_back(Collision{
                    v,
                    epa.GetDirection(),
                    Dot(v, epa.GetDirection()) - projection_min,
            });
        }
    }
    for (const Vec3 &v : p2->GetVertices()) {
        if (Dot(v, epa.GetDirection()) <= projection_max && p1->IsPointInside(v)) {
            collisions.push_back(Collision{
                    v,
                    epa.GetDirection(),
                    projection_max - Dot(v, epa.GetDirection()),
            });
        }
    }

    if (!collisions.empty()) {
        return;
    }

    // Edge to Edge case. Corner case

    Vec3 axis1 = Cross(epa.GetDirection(), Vec3::RandomUnit()).Norm();
    Vec3 axis2 = Cross(epa.GetDirection(), axis1).Norm();
    std::vector<Vec3> directions = {
            epa.GetDirection() + axis1 * 1e-5,
            epa.GetDirection() + axis2 * 1e-5,
            epa.GetDirection() - axis1 * 1e-5,
            epa.GetDirection() - axis2 * 1e-5,
    };
    std::set<Vec3> edge1;
    std::set<Vec3> edge2;
    for (auto dir : directions) {
        edge1.insert(p1->GetSupportingVector(dir));
        edge2.insert(p2->GetSupportingVector(-dir));
    }
    ENGINE_ASSERT(edge1.size() == 2 && edge2.size() == 2, "Edges... are not edges... :(");
    if (edge1.size() != 2 || edge2.size() != 2) {
        return;
    }
    Vec3 P1 = Vec3(Dot(axis1, *edge1.begin()), Dot(axis2, *edge1.begin()), 0);
    Vec3 P2 = Vec3(Dot(axis1, *(++edge1.begin())), Dot(axis2, *(++edge1.begin())), 0);
    Vec3 V1 = Vec3(Dot(axis1, *edge2.begin()), Dot(axis2, *edge2.begin()), 0);
    Vec3 V2 = Vec3(Dot(axis1, *(++edge2.begin())), Dot(axis2, *(++edge2.begin())), 0);
    double div = Cross(P2 - P1, V2 - V1).z;
    ENGINE_ASSERT(fabs(div) >= 1e-6, "Div almost zero");
    if (fabs(div) < 1e-6) {
        return;
    }
    // (p1 ----- p2)
    // (v1 ----- v2)
    // Cross(p2-p1, td + v1 - p1) = 0
    // Cross(p2-p1, td) + Cross(p2-p1, v1 - p1) = 0
    // tCross(p2-p1, v2-v1) + Cross(p2-p1, v1-p1) = 0
    double t = -Cross(P2 - P1, V1 - P1).z / div;

    Vec3 PV = (V2 - V1) * t + V1;
    Vec3 res = PV.x * axis1 + PV.y * axis2 + epa.GetDirection() * (projection_max + projection_min) * 0.5;
    collisions.push_back(Collision{
        res,
        epa.GetDirection(),
        epa.GetDistance()
    });
}

const std::vector<Collision> &CollisionDetector::GetCollisionPoints() const {
    return collisions;
}

bool CollisionDetector::HasCollision() const {
    return !collisions.empty();
}


GilbertJohnsonKeerthi::GilbertJohnsonKeerthi(const ConvexShape *a,
                                             const ConvexShape *b) {
    auto difference = std::make_unique<MinkowskiDifferenceShape>(a, b);
    RunAlgorithm(difference.get());
}

GilbertJohnsonKeerthi::GilbertJohnsonKeerthi(const ConvexShape *volume) {
    RunAlgorithm(volume);
}

std::array<Vec3, 4> GilbertJohnsonKeerthi::GetSimplex() const {
    return simplex;
}

bool GilbertJohnsonKeerthi::ContainsOrigin() const {
    return contains_origin;
}

void GilbertJohnsonKeerthi::RunAlgorithm(const ConvexShape *shape) {
    contains_origin = false;

    Vec3 b0 = Vec3(1, 0, 0);
    Vec3 b1 = Vec3(-0.5, 0, sqrt(3) / 2);
    Vec3 b2 = Vec3(-0.5, 0, -sqrt(3) / 2);
    bool simplex_found = false;
    for (int iter = 0; iter < 100 && !simplex_found; iter++) {
        Mat3 rot = Quat::RandomRotation().Mat();
        simplex[0] = shape->GetSupportingVector(rot * b0);
        simplex[1] = shape->GetSupportingVector(rot * b1);
        simplex[2] = shape->GetSupportingVector(rot * b2);
        double volume = Dot(simplex[0], Cross(simplex[1], simplex[2]));
        if (fabs(volume) > GJK_MIN_STARTING_VOLUME) {
            Vec3 normal = Cross(simplex[1] - simplex[0], simplex[2] - simplex[1]);
            if (Dot(normal, -simplex[0]) < 0) {
                normal *= -1;
                std::swap(simplex[0], simplex[1]);
            }
            simplex[3] = shape->GetSupportingVector(normal);
            simplex_found = true;
        }
    }

    if (!simplex_found) {
        return;
    }

    for (int iter = 0; iter < 300; iter++) {
        Vec3 da = simplex[3] - simplex[0];
        Vec3 db = simplex[3] - simplex[1];
        Vec3 dc = simplex[3] - simplex[2];

        Vec3 d0 = -simplex[3];

        Vec3 abdNorm = Cross(da, db);
        Vec3 bcdNorm = Cross(db, dc);
        Vec3 cadNorm = Cross(dc, da);

        Vec3 normal = Vec3();

        if (Dot(abdNorm, d0) > 0) {
            std::swap(simplex[2], simplex[3]);
            normal = abdNorm;
        } else if (Dot(bcdNorm, d0) > 0) {
            std::swap(simplex[0], simplex[1]);
            std::swap(simplex[1], simplex[2]);
            std::swap(simplex[2], simplex[3]);
            normal = bcdNorm;
        } else if (Dot(cadNorm, d0) > 0) {
            std::swap(simplex[1], simplex[3]);
            normal = cadNorm;
        } else {
            contains_origin = true;
            return;
        }


        simplex[3] = shape->GetSupportingVector(normal);
        if (Dot(simplex[3], normal) < 0) {
            return;
        }
    }
}

void ExpandingPolytopeAlgorithm::RunAlgorithm(const ConvexShape *shape, std::array<Vec3, 4> starting_simplex) {
    std::vector<Vec3> vertices(starting_simplex.begin(), starting_simplex.end());
    std::vector<std::array<int, 3>> vert_id(4);
    std::vector<std::array<int, 3>> adj_id(4);
    vert_id[0] = {0, 1, 3};
    vert_id[1] = {1, 2, 3};
    vert_id[2] = {2, 0, 3};
    vert_id[3] = {1, 0, 2};
    adj_id[0] = {1, 2, 3};
    adj_id[1] = {2, 0, 3};
    adj_id[2] = {0, 1, 3};
    adj_id[3] = {2, 1, 0};

    std::vector<int> face_deleted(4, 0);
    std::queue<int> face_pool;

    for (int iter = 0; iter < 50; iter++) {
        int nearest_face = -1;
        Vec3 nearest_face_normal;
        double nearest_face_distance = std::numeric_limits<double>::max();
        for (int face = 0; face < vert_id.size(); face++) {
            if (face_deleted[face]) {
                continue;
            }
            Vec3 v0 = vertices[vert_id[face][0]];
            Vec3 v1 = vertices[vert_id[face][1]];
            Vec3 v2 = vertices[vert_id[face][2]];
            Vec3 norm = Cross(v1 - v0, v2 - v0).Norm();
            double cur_distance = Dot(v0, norm);
            if (cur_distance < nearest_face_distance) {
                nearest_face = face;
                nearest_face_distance = cur_distance;
                nearest_face_normal = norm;
            }
        }
        ENGINE_ASSERT(nearest_face != -1, "EPA Nearest face not found");
        if (nearest_face == -1) {
            return;
        }

        Vec3 supporting_vertex = shape->GetSupportingVector(nearest_face_normal);
        if (fabs(Dot(supporting_vertex, nearest_face_normal) - nearest_face_distance) < EPA_DISTANCE_EPSILON) {
            distance = nearest_face_distance;
            direction = nearest_face_normal;
            answer_found = true;
            return;
        }

        std::vector<bool> face_visited(vert_id.size());

        std::queue<int> q;
        q.push(nearest_face);
        face_visited[nearest_face] = true;

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
                if (volume > 0) {
                    int opposite_side = 0;
                    for (; opposite_side < 3 && adj_id[adj_face][opposite_side] != cur_face; opposite_side++) {}
                    ENGINE_ASSERT(opposite_side < 3, "EPA: Faces are not connected");
                    if (opposite_side >= 3) {
                        return;
                    }
                    ridges.emplace_back(cur_face, side, adj_face, opposite_side);
                } else if (!face_visited[adj_face]) {
                    face_visited[adj_face] = true;
                    q.push(adj_face);
                }
            }
        }
        std::vector<int> new_face_ids;
        for (auto[old_face_id, old_side, face_id, side] : ridges) {
            int new_face_id;
            if (face_pool.empty()) {
                new_face_id = vert_id.size();
                vert_id.push_back({});
                adj_id.push_back({});
                face_deleted.push_back(false);
            } else {
                new_face_id = face_pool.front();
                face_pool.pop();
                face_deleted[new_face_id] = false;
            }
            auto &new_adj_id = adj_id[new_face_id];
            auto &new_vert_id = vert_id[new_face_id];

            new_vert_id = vert_id[old_face_id];
            new_vert_id[old_side] = vertices.size();

            new_adj_id[old_side] = face_id;
            adj_id[face_id][side] = new_face_id;

            new_face_ids.push_back(new_face_id);
        }
        std::map<std::pair<int, int>, std::pair<int, int>> ridge_map;
        for (int face_id : new_face_ids) {
            for (int side = 0; side < 3; side++) {
                std::pair<int, int> ridge = {vert_id[face_id][(side + 1) % 3], vert_id[face_id][(side + 2) % 3]};
                if (ridge.first > ridge.second) {
                    std::swap(ridge.first, ridge.second);
                }
                if (ridge_map.find(ridge) != ridge_map.end()) {
                    auto[other_face, other_side] = ridge_map[ridge];
                    adj_id[other_face][other_side] = face_id;
                    adj_id[face_id][side] = other_face;
                } else {
                    ridge_map[ridge] = {face_id, side};
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

ExpandingPolytopeAlgorithm::ExpandingPolytopeAlgorithm(const ConvexShape *shape, std::array<Vec3, 4> starting_simplex) {
    RunAlgorithm(shape, starting_simplex);
}

ExpandingPolytopeAlgorithm::ExpandingPolytopeAlgorithm(const ConvexShape *a, const ConvexShape *b,
                                                       std::array<Vec3, 4> starting_simplex) {
    auto difference = std::make_unique<MinkowskiDifferenceShape>(a, b);
    RunAlgorithm(difference.get(), starting_simplex);
}

const Vec3 &ExpandingPolytopeAlgorithm::GetDirection() const {
    return direction;
}

double ExpandingPolytopeAlgorithm::GetDistance() const {
    return distance;
}

bool ExpandingPolytopeAlgorithm::AnswerFound() const {
    return answer_found;
}
