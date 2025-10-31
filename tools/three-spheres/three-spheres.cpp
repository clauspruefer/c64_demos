//- OpenGL Three Spheres Display
//- Displays 3 static spheres at different sizes and Y positions

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>

using namespace std;

// Sphere properties - positioned to overlap and form a single 3D model
namespace Spheres {
    // Sphere 1: Big, at top
    const float SPHERE1_RADIUS = 1.5f;
    const float SPHERE1_Y = 1.5f;  // Positioned to extend from 0 to 3.0
    
    // Sphere 2: Medium, in middle - overlaps with Sphere 1 (increased by 70%)
    const float SPHERE2_RADIUS = 1.02f;
    const float SPHERE2_Y = -0.3f;  // Overlaps with Sphere 1 (extends from -1.32 to 0.72)
    
    // Sphere 3: Large, at bottom - overlaps with Sphere 2 (increased by 40%)
    const float SPHERE3_RADIUS = 1.4f;
    const float SPHERE3_Y = -1.5f;  // Overlaps with Sphere 2 (extends from -2.9 to -0.1)
    
    void drawSphere(float x, float y, float z, float radius, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);
        glutSolidSphere(radius, 32, 32);  // 32 slices and stacks for smooth appearance
        glPopMatrix();
    }
    
    void drawAll() {
        // Sphere 1: Big, top position, red color
        drawSphere(0.0f, SPHERE1_Y, 0.0f, SPHERE1_RADIUS, 0.9f, 0.2f, 0.2f);
        
        // Sphere 2: Small, middle position, green color
        drawSphere(0.0f, SPHERE2_Y, 0.0f, SPHERE2_RADIUS, 0.2f, 0.9f, 0.2f);
        
        // Sphere 3: Medium, bottom position, blue color
        drawSphere(0.0f, SPHERE3_Y, 0.0f, SPHERE3_RADIUS, 0.2f, 0.2f, 0.9f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Position camera to view the combined 3D model
    gluLookAt(
        5.0, 0.0, 8.0,   // Eye position - slightly to the right
        0.0, 0.0, 0.0,   // Look at center of the model
        0.0, 1.0, 0.0    // Up vector
    );
    
    // Draw all three spheres
    Spheres::drawAll();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable lighting for better visualization
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light position
    GLfloat light_position[] = { 5.0, 5.0, 5.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Material properties for shiny spheres
    GLfloat mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Set background color
    glClearColor(0.1, 0.1, 0.15, 1.0);
    
    cout << "Three Spheres Display initialized!" << endl;
    cout << "Sphere 1: Radius = " << Spheres::SPHERE1_RADIUS << ", Y position = " << Spheres::SPHERE1_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE1_Y - Spheres::SPHERE1_RADIUS) << " to " << (Spheres::SPHERE1_Y + Spheres::SPHERE1_RADIUS) << endl;
    cout << "Sphere 2: Radius = " << Spheres::SPHERE2_RADIUS << ", Y position = " << Spheres::SPHERE2_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE2_Y - Spheres::SPHERE2_RADIUS) << " to " << (Spheres::SPHERE2_Y + Spheres::SPHERE2_RADIUS) << endl;
    cout << "Sphere 3: Radius = " << Spheres::SPHERE3_RADIUS << ", Y position = " << Spheres::SPHERE3_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE3_Y - Spheres::SPHERE3_RADIUS) << " to " << (Spheres::SPHERE3_Y + Spheres::SPHERE3_RADIUS) << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Three Spheres - OpenGL Display");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
