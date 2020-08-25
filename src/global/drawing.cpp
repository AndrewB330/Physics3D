#include "GL/freeglut.h"
#include "drawing.h"
#include "../model/polyhedron.h"
#include "../model/sphere.h"

namespace Graphics {
    void glVec3(const Vec3& v) {
        glVertex3f(v.x, v.y, v.z);
    }

    void glMaterial(const Material& material) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient.data);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse.data);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular.data);
    }

    void DrawPolyhedron(const Polyhedron* p) {
        glMaterial(p->GetMaterial());

        glBegin(GL_TRIANGLES);
        const auto& vertices = p->GetVertices();
        for (const auto& face : p->GetFaces()) {
            glNormal3f(face.normal.x, face.normal.y, face.normal.z);
            glVec3(vertices[face.v0]);
            glVec3(vertices[face.v1]);
            glVec3(vertices[face.v2]);
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
        if (auto p = dynamic_cast<const Polyhedron*>(object)) {
            DrawPolyhedron(p);
        }
        if (auto s = dynamic_cast<const Sphere*>(object)) {
            DrawSphere(s);
        }
    }
}
