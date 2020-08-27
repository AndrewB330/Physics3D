#include "GL/freeglut.h"
#include <viewer/drawing.hpp>
#include <engine/model/objects/polyhedron.hpp>
#include <engine/model/objects/sphere.hpp>
#include <engine/model/objects/box.hpp>

namespace Graphics {
    void glVec3(const Vec3& v) {
        glVertex3f(v.x, v.y, v.z);
    }

    void glMaterial(const Material& material) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient.data);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse.data);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular.data);
    }

    void DrawBBox(const BoundingBox& bbox) {
        auto vertices = GetBoxVertices(bbox);
        Material material;
        material.ambient = material.diffuse = Color(0.99f, 0.2f, 0.1f);
        glMaterial(material);
        glLineWidth(2);
        glNormal3f(0, 0, 0);
        glBegin(GL_LINES);
        for (const auto& face : GetBoxFaces()) {
            glVec3(vertices[face.v0]);
            glVec3(vertices[face.v1]);
            glVec3(vertices[face.v1]);
            glVec3(vertices[face.v2]);
        }
        glEnd();
    }

    void DrawPolyhedron(const Polyhedron* p) {
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

    void DrawObject(const Object* object) {
        if (object->collided) {
            DrawBBox(object->GetBBox());
        }
        if (auto p = dynamic_cast<const Polyhedron*>(object)) {
            DrawPolyhedron(p);
        }
        if (auto s = dynamic_cast<const Sphere*>(object)) {
            DrawSphere(s);
        }
    }
}
