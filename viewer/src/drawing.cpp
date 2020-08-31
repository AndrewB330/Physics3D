#include "GL/freeglut.h"
#include <viewer/drawing.hpp>
#include <engine/utils.hpp>

namespace Graphics {
    void glVec3(const Vec3 &v) {
        glVertex3f(v.x, v.y, v.z);
    }

    void glMaterial(const RenderMaterial &material) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient.data);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse.data);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular.data);
    }

    void DrawBBox(const BoundingBox &bbox) {
        auto vertices = GetBoxVertices(bbox);
        RenderMaterial material;
        material.ambient = material.diffuse = Color(0.99f, 0.2f, 0.1f);
        glMaterial(material);
        glLineWidth(2);
        glNormal3f(0, 0, 0);
        glBegin(GL_LINES);
        for (auto[v0, v1, v2] : GetBoxFaces()) {
            glVec3(vertices[v0]);
            glVec3(vertices[v1]);
            glVec3(vertices[v1]);
            glVec3(vertices[v2]);
        }
        glEnd();
    }


    void DrawCollider(const Collider *collider, const RenderMaterial &material) {
        //DrawBBox(collider->GetBBox());

        glMaterial(material);

        auto orig = collider->GetOrigShape();

        glPushMatrix();
        auto position = collider->GetTranslation();
        Quat q = collider->GetRotation();
        double angle = acos(std::min(std::max(-1.0, q.s), 1.0)) * 2 / PI * 180;
        glTranslatef(position.x, position.y, position.z);
        glRotatef(angle, q.v.x, q.v.y, q.v.z);

        if (auto s = dynamic_cast<const BoxShape*>(orig)) {
            Vec3 size = orig->GetSupportingVector(Vec3(1,1,1))*2;
            const auto &vertices = GetBoxVertices(size.x, size.y, size.z);
            glLineWidth(4);
            glBegin(GL_TRIANGLES);
            for (auto[v0, v1, v2] : GetBoxFaces()) {
                Vec3 normal = Cross(vertices[v1] - vertices[v0], vertices[v2] - vertices[v0]).Norm();
                glNormal3f(normal.x, normal.y, normal.z);
                glVec3(vertices[v0]);
                glVec3(vertices[v1]);
                glVec3(vertices[v2]);
            }
            glEnd();
        } else {
            double radius = orig->GetSupportingVector(Vec3(1,0,0)).Len();
            glutSolidSphere(radius, 32, 32);
            glBegin(GL_LINES);
            glVec3(Vec3());
            glVec3(Vec3(+1, 0, 0) * (radius + 0.1));
            glVec3(Vec3());
            glVec3(Vec3(-1, 0, 0) * (radius + 0.1));
            glVec3(Vec3());
            glVec3(Vec3(0, +1, 0) * (radius + 0.1));
            glVec3(Vec3());
            glVec3(Vec3(0, -1, 0) * (radius + 0.1));
            glVec3(Vec3());
            glVec3(Vec3(0, 0, +1) * (radius + 0.1));
            glVec3(Vec3());
            glVec3(Vec3(0, 0, -1) * (radius + 0.1));
            glEnd();
        }
        glPopMatrix();
    }

/*    void DrawPolyhedron(const Polyhedron* p) {
        glMaterial(p->GetMaterial());

        glBegin(GL_TRIANGLES);
        const auto& vertices = p->GetVertices();
        const auto& faces = p->GetFaces();
        const auto& normals = p->GetNormals();
        for (int i = 0; i < faces.size(); i++) {
            glNormal3f(normals[i].x, normals[i].y, normals[i].z);
            glVec3(vertices[faces[i].v0]);
            glVec3(vertices[faces[i].v1]);
            glVec3(vertices[faces[i].v2]);
        }
        glEnd();
    }

    void DrawSphere(const Sphere* s) {
        glPushMatrix();
        auto position = s->GetState().position;
        glTranslatef(position.x, position.y, position.z);

        glMaterial(s->GetMaterial());

        glutSolidSphere(s->GetRadius(), 32, 32);
        glPopMatrix();
    }

    void DrawObject(const PhysObject* object) {
        if (object->collided) {
            DrawBBox(object->GetBBox());
        }
        if (auto p = dynamic_cast<const Polyhedron*>(object)) {
            DrawPolyhedron(p);
        }
        if (auto s = dynamic_cast<const Sphere*>(object)) {
            DrawSphere(s);
        }
    }*/
}
