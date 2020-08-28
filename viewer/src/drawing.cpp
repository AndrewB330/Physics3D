#include "GL/freeglut.h"
#include <viewer/drawing.hpp>

namespace Graphics {
    void glVec3(const Vec3& v) {
        glVertex3f(v.x, v.y, v.z);
    }

    void glMaterial(const RenderMaterial& material) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient.data);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse.data);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular.data);
    }

    void DrawBBox(const BoundingBox& bbox) {
        auto vertices = GetBoxVertices(bbox);
        RenderMaterial material;
        material.ambient = material.diffuse = Color(0.99f, 0.2f, 0.1f);
        glMaterial(material);
        glLineWidth(2);
        glNormal3f(0, 0, 0);
        glBegin(GL_LINES);
        for (auto [v0, v1, v2] : GetBoxFaces()) {
            glVec3(vertices[v0]);
            glVec3(vertices[v1]);
            glVec3(vertices[v1]);
            glVec3(vertices[v2]);
        }
        glEnd();
    }


    void DrawShape(const Shape* shape, const RenderMaterial& material) {
        glMaterial(material);
        if (auto p = dynamic_cast<const ConvexPolyhedronShape*>(shape)) {
            const auto & vertices = p->GetVertices();
            glLineWidth(4);
            glBegin(GL_TRIANGLES);
            for(auto [v0, v1, v2] : p->GetFaces()) {
                Vec3 normal = Cross(vertices[v1] - vertices[v0], vertices[v2] - vertices[v0]).Norm();
                glNormal3f(normal.x, normal.y, normal.z);
                glVec3(vertices[v0]);
                glVec3(vertices[v1]);
                glVec3(vertices[v2]);
            }
            glEnd();
        } else if (auto s = dynamic_cast<const SphereShape*>(shape)) {
            glPushMatrix();
            auto position = s->GetTranslation();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(s->GetRadius(), 32, 32);
            glPopMatrix();
        }
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
