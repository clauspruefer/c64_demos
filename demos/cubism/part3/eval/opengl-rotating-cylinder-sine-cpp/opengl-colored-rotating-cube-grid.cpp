//- Taken from OpenGL examples, used for c64-Demo precalculated colored cylinder with sine wave wrapping

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>

#include <glm/vec3.hpp>

using namespace std;

static const int FPS = 60;
static GLint rotateAngle = 0;
static GLint rotateAngle2 = 0;

//- Cylinder Namespace
namespace Cylinder {

    const int NUM_VERTICES = 108;
    const int NUM_PYRAMID_APEXES = 6;

    //- Cylinder vertices: virtual rope wrapping around cylinder with sine wave modulation
    //- Improved with larger radius (~8 units) and height (30 units) for better visibility
    //- Increased to 108 vertices for smoother appearance
    GLint vertices[NUM_VERTICES][3] = {
      {  8,  15,   0}, {  8,  15,   3}, {  7,  15,   5}, {  6,  14,   7}, 
      {  3,  14,   8}, {  1,  14,   8}, { -2,  14,   8}, { -5,  14,   7}, 
      { -7,  13,   6}, { -8,  13,   4}, { -9,  13,   2}, { -9,  12,   0}, 
      { -9,  12,  -2}, { -7,  11,  -4}, { -6,  11,  -6}, { -4,  11,  -7}, 
      { -2,  10,  -7}, {  0,  10,  -8}, {  2,  10,  -7}, {  3,   9,  -7}, 
      {  5,   9,  -6}, {  5,   9,  -5}, {  6,   8,  -4}, {  6,   8,  -2}, 
      {  7,   8,  -1}, {  6,   8,   1}, {  6,   8,   3}, {  6,   7,   5}, 
      {  5,   7,   6}, {  3,   7,   7}, {  2,   7,   8}, {  0,   7,   8}, 
      { -2,   6,   8}, { -4,   6,   7}, { -6,   6,   5}, { -8,   6,   3}, 
      { -9,   5,   1}, { -9,   5,  -2}, { -9,   5,  -4}, { -7,   4,  -6}, 
      { -5,   4,  -8}, { -2,   3,  -8}, {  0,   3,  -8}, {  3,   3,  -7}, 
      {  6,   2,  -6}, {  7,   2,  -4}, {  8,   2,  -2}, {  8,   1,   0}, 
      {  7,   1,   2}, {  6,   1,   4}, {  4,   1,   5}, {  2,   0,   6}, 
      {  0,   0,   7}, { -2,   0,   7}, { -3,   0,   7}, { -5,   0,   6}, 
      { -6,   0,   5}, { -7,  -1,   3}, { -7,  -1,   1}, { -7,  -1,   0}, 
      { -7,  -1,  -2}, { -6,  -2,  -4}, { -5,  -2,  -6}, { -4,  -2,  -7}, 
      { -3,  -3,  -9}, { -1,  -3,  -9}, {  0,  -3,  -9}, {  2,  -4,  -9}, 
      {  4,  -4,  -8}, {  6,  -5,  -6}, {  7,  -5,  -5}, {  8,  -5,  -3}, 
      {  9,  -6,   0}, {  9,  -6,   2}, {  8,  -6,   4}, {  7,  -6,   5}, 
      {  5,  -7,   6}, {  3,  -7,   7}, {  1,  -7,   7}, { -2,  -7,   7}, 
      { -4,  -7,   6}, { -6,  -8,   5}, { -7,  -8,   3}, { -7,  -8,   1}, 
      { -7,  -8,  -1}, { -6,  -8,  -3}, { -5,  -9,  -4}, { -3,  -9,  -6}, 
      { -1,  -9,  -7}, {  1, -10,  -7}, {  3, -10,  -7}, {  5, -10,  -6}, 
      {  7, -11,  -5}, {  8, -11,  -3}, {  8, -11,   0}, {  8, -12,   2}, 
      {  7, -12,   5}, {  6, -13,   7}, {  4, -13,   8}, {  2, -13,   9}, 
      {  0, -14,   9}, { -2, -14,   9}, { -3, -14,   8}, { -5, -14,   7}, 
      { -6, -14,   5}, { -7, -15,   3}, { -8, -15,   1}, { -8, -15,   0}
    };

    GLfloat vertexColors[NUM_VERTICES][3] = {
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
    };

    void draw() {
      // Draw cylinder vertices as points
      glBegin(GL_POINTS);
      for (int i = 0; i < NUM_VERTICES; i++) {
        glColor3fv((GLfloat*)&vertexColors[i]);
        glVertex3iv((GLint*)&vertices[i]);
      }
      glEnd();
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(rotateAngle, 0.0, 0.1, 0.0);

    Cylinder::draw();
    rotateAngle += 1;
    
    GLdouble retX, retY, retZ;

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

    //- Project all cylinder vertices to window coordinates using gluProject
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble projectedCoords[Cylinder::NUM_VERTICES][3];
    
    for (int i = 0; i < Cylinder::NUM_VERTICES; i++) {
        gluProject(
            (GLdouble)Cylinder::vertices[i][0], 
            (GLdouble)Cylinder::vertices[i][1], 
            (GLdouble)Cylinder::vertices[i][2],
            modelview, 
            projection, 
            viewport,
            &projectedCoords[i][0], 
            &projectedCoords[i][1], 
            &projectedCoords[i][2]
        );
        cout << "Vertex " << i << " -> Window coords: (" 
             << projectedCoords[i][0] << ", " 
             << projectedCoords[i][1] << ", " 
             << projectedCoords[i][2] << ")" << endl;
    }

    glPopMatrix();
    glFlush();

    glutSwapBuffers();
}

void timer(int v) {
  glLoadIdentity();
  // Position camera to view the complete cylinder in 420x2520 window
  // Cylinder: Y from -15 to +15 (height 30), X/Z from -9 to +9 (diameter ~18)
  // Camera positioned along Z-axis, centered on cylinder at (0, 0, 0)
  gluLookAt(0.0, 0.0, 50.0,    // Eye position (far enough to see full cylinder)
            0.0, 0.0, 0.0,      // Look at center of cylinder
            0.0, 1.0, 0.0);     // Up vector
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Adjust perspective to properly frame cylinder in tall window (420x2520)
  // FOV adjusted to ensure full cylinder visibility
  gluPerspective(45.0, GLfloat(w) / GLfloat(h), 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(420, 2520);
  glutCreateWindow("Precalculated Cylinder C64 Demo");
  //glutFullScreen();
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
