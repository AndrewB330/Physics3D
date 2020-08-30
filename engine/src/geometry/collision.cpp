#include <engine/geometry/collision.hpp>
#include <queue>

struct Support {
    Vec3 a, b, v;
};

Support support(const ConvexShape *a, const ConvexShape *b, Vec3 dir) {
    Support s;
    s.a = a->GetSupportingVector(-dir);
    s.b = b->GetSupportingVector(dir);
    s.v = s.b - s.a;
    return s;
}

CollisionDetector::CollisionDetector(const ConvexShape *a, const ConvexShape *b) {
    if (!IsIntersect(a->GetBBox(), b->GetBBox())) {
        return;
    }

    Support v[5];

    v[0].a = a->GetTranslation();
    v[0].b = b->GetTranslation();
    v[0].v = v[0].b - v[0].a;

    if (v[0].v.LenSqr() < 1e-6) v[0].v = Vec3(0.00001, 0, 0);

    Vec3 n = -v[0].v;
    v[1] = support(a, b, n);
    if (Dot(v[1].v, n) <= 0.0) {
        return;
    }

    n = Cross(v[1].v,v[0].v);
    if (v[0].v.LenSqr() < 1e-6) {
        Collision c;
        c.point = (v[1].a + v[1].b) * 0.5;
        c.normal = (v[1].b - v[0].a).Norm();
        c.penetration = 0;
        collisions.push_back(c);
        return;
    }

    v[2] = support(a, b, n);
    if (Dot(v[2].v, n) <= 0.0) return;

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
        if (Dot(v[3].v, n) <= 0.0f) return;

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

            if (delta <= 0.001f || depth >= 0 || pass > 10) {
                if (!hit) return;

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
    Vec3 ap = n * Dot(s.a - p1, n) + p1;
    Vec3 bp = n * Dot(s.b - p2, n) + p2;

    Collision c;
    c.point = (ap + bp) * 0.5;
    c.normal = n.Norm();
    c.penetration = Dot(n.Norm(), v[4].v);
    if (c.penetration > 0.5) {
        int x = 5;
    }
    collisions.push_back(c);
}

const std::vector<Collision> &CollisionDetector::GetCollisionPoints() const {
    return collisions;
}

bool CollisionDetector::HasCollision() const {
    return !collisions.empty();
}
