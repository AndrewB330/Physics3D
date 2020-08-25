#include "GL/freeglut.h"
#include "drawing.h"
#include "../model/polyhedron.h"

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
        glPushMatrix();
        glTranslatef(p->state.position.x, p->state.position.y, p->state.position.z);

        glMaterial(p->material);

        glBegin(GL_TRIANGLES);
        for (const auto& face : p->faces) {
            glNormal3f(face.normal.x, face.normal.y, face.normal.z);
            glVec3(p->vertices[face.v0]);
            glVec3(p->vertices[face.v1]);
            glVec3(p->vertices[face.v2]);
        }
        glEnd();
        glPopMatrix();
    }

    void DrawObject(const Object* object) {
        if (auto p = dynamic_cast<const Polyhedron*>(object)) {
            DrawPolyhedron(p);
        }
    }
}
