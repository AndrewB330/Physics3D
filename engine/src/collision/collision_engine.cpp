#include <engine/collision/collision_engine.hpp>

CollisionEngine::CollisionEngine() {}

void CollisionEngine::AddCollider(int id, std::shared_ptr<const Collider> collider) {
    colliders[id] = collider;
}

bool CollisionEngine::HasColliderId(int id) const {
    return colliders.find(id) != colliders.end();
}

void CollisionEngine::RemoveCollider(int id) {
    colliders.erase(colliders.find(id));
}

std::vector<Collision> CollisionEngine::GetAllCollisions() {
    std::vector<Collision> collisions;
    tree.Rebuild(colliders);
    for(const auto & [id, collider] : colliders) {
        auto candidates = tree.GetCandidateIds(collider->GetBBox());
        for(int other_id : candidates) {
            if (id >= other_id) {
                continue;
            }
            auto collision = GetCollision(id, other_id);
            if (collision.has_value()) {
                collisions.push_back(collision.value());
            }
        }
    }
    return collisions;
}

struct Support {
    Vec3 a,b,v;
};

Support support(const std::shared_ptr<const Collider> & a, const std::shared_ptr<const Collider> & b, const Vec3& dir) {
    Support s;
    s.a = a->GetSupportingVector(-dir);
    s.b = b->GetSupportingVector(dir);
    s.v = s.b - s.a;
    return s;
}

std::optional<Collision> CollisionEngine::GetCollision(int a_id, int b_id) const {
    // todo add check
    const std::shared_ptr<const Collider> & a = colliders.find(a_id)->second;
    const std::shared_ptr<const Collider> & b = colliders.find(b_id)->second;
    Support v[5];

    v[0].a = a->GetTranslation();
    v[0].b = b->GetTranslation();
    v[0].v = v[0].b - v[0].a;

    if (v[0].v.LenSqr() < 1e-6) v[0].v = Vec3(0.00001, 0, 0);

    Vec3 n = -v[0].v;
    v[1] = support(a, b, n);
    if (Dot(v[1].v, n) <= 0.0) {
        return std::nullopt;
    }

    n = Cross(v[1].v,v[0].v);
    if (v[0].v.LenSqr() < 1e-6) {
        Collision collision;
        collision.a_id = a_id;
        collision.b_id = b_id;
        collision.point_a = v[1].a;
        collision.point_b = v[1].b;
        collision.normal = (v[1].b - v[0].a).Norm();
        collision.penetration = (v[1].b - v[0].a).Len();
        return collision;
    }

    v[2] = support(a, b, n);
    if (Dot(v[2].v, n) <= 0.0) {
        return std::nullopt;
    }

    n = Cross(v[1].v - v[0].v,v[2].v - v[0].v);
    float dist = Dot(n, v[0].v);

    if (dist > 0.0) {
        Support tmp = v[1];
        v[1] = v[2];
        v[2] = tmp;
        n = -n;
    }

    Vec3 p1, p2;
    bool hit = false;
    while (!hit) {
        v[3] = support(a, b, n);
        if (Dot(v[3].v, n) <= 0.0f) {
            return std::nullopt;
        }

        if (Dot(Cross(v[1].v,v[3].v), v[0].v) < 0.0f) {
            v[2] = v[3];
            n = Cross(v[1].v - v[0].v, v[3].v - v[0].v);
            continue;
        }

        if (Dot(Cross(v[3].v, v[2].v), v[0].v) < 0.0f) {
            v[1] = v[3];
            n = Cross(v[3].v - v[0].v, v[2].v - v[0].v);
            continue;
        }

        int pass = 0;
        while (1) {
            pass++;

            n = Cross(v[2].v - v[1].v, v[3].v - v[1].v).Norm();
            float d = Dot(n,v[1].v);

            if (d >= 0)
                hit = true;

            v[4] = support(a, b, n);
            float delta = Dot(v[4].v - v[3].v, n);
            float depth = -Dot(n, v[4].v);

            if (delta <= 0.001f || depth >= 0 || pass > 30) {
                if (!hit) {
                    return std::nullopt;
                }

                float bar[4];
                bar[0] = Dot(Cross(v[1].v, v[2].v), v[3].v);
                bar[1] = Dot(Cross(v[3].v, v[2].v), v[0].v);
                bar[2] = Dot(Cross(v[0].v, v[1].v), v[3].v);
                bar[3] = Dot(Cross(v[2].v, v[1].v), v[0].v);

                float sum = bar[0] + bar[1] + bar[2] + bar[3];

                if (sum <= 0) {
                    bar[0] = 0.0f;
                    bar[1] = Dot(Cross(v[2].v, v[3].v), n);
                    bar[2] = Dot(Cross(v[3].v, v[1].v), n);
                    bar[3] = Dot(Cross(v[1].v, v[2].v), n);
                    sum = bar[1] + bar[2] + bar[3];
                }

                float inv = 1.0f / sum;

                p1 = (v[0].a * bar[0] + v[1].a * bar[1] + v[2].a * bar[2] + v[3].a * bar[3]) * inv;
                p2 = (v[0].b * bar[0] + v[1].b * bar[1] + v[2].b * bar[2] + v[3].b * bar[3]) * inv;

                break;
            }

            float d1 = Dot(Cross(v[4].v, v[1].v), v[0].v);
            float d2 = Dot(Cross(v[4].v, v[2].v), v[0].v);
            float d3 = Dot(Cross(v[4].v, v[3].v), v[0].v);

            if (d1 < 0)
                v[d2 < 0 ? 1 : 3] = v[4];
            else
                v[d3 < 0 ? 2 : 1] = v[4];
        }
    }

    Support s;
    s = support(a, b, n);

    Collision collision;
    collision.a_id = a_id;
    collision.b_id = b_id;
    collision.point_a = n * Dot(s.a - p1, n) + p1;
    collision.point_b = n * Dot(s.b - p2, n) + p2;
    collision.normal = n.Norm();
    collision.penetration = Dot(n.Norm(), v[4].v);
    return collision;
}
