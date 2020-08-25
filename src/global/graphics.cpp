#include "graphics.h"
#include <GL\freeglut.h>

namespace Graphics {

    bool initialized = false;
    int window_width = 0;
    int window_height = 0;
    void(*render_func)() = nullptr;

    void ReshapeFunc(int width, int height) {
        GLfloat aspect = (GLfloat)width / (GLfloat)height;

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, aspect, 0.01f, 1000.0f);

        window_width = width;
        window_height = height;
    }

    void RenderWrapper() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        if (render_func != nullptr) {
            render_func();
        }
        glutSwapBuffers();
    }

    void InitLightning() {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat specular[] = { 0.3, 0.3, 0.3, 0.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
        glLightfv(GL_LIGHT0, GL_SPECULAR, ambient);

        GLfloat lightpos[] = { 0.5f, 5.5f, 3.0f, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    }

    void InitWindow(int & argc, char **argv, const std::string& name, int width, int height, int left, int top) {
        if (initialized) {
            return;
        }
        window_width = width;
        window_width = height;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE);
        glutInitWindowSize(width, height);
        glutInitWindowPosition(left, top);
        glutCreateWindow(name.c_str());
        glutDisplayFunc(RenderWrapper);
        glutReshapeFunc(ReshapeFunc);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
        glClearDepth(1.0f);                   // Set background depth to farthest

        glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
        glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
        glShadeModel(GL_SMOOTH);   // Enable smooth shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

        InitLightning();

        initialized = true;
    }

    void SetRenderFunction(void func()) {
        render_func = func;
    }

    void Redraw() {
        glutMainLoopEvent();
        glutPostRedisplay();
    }

    int GetWidth() {
        return window_width;
    }

    int GetHeight() {
        return window_height;
    }

}
