//- Taken from OpenGL examples, used for c64-Demo precalculated DNA double helix

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

//- DNA Helix Namespace
namespace DNAHelix {

    const int NUM_VERTICES = 54;
    const int NUM_PYRAMID_APEXES = 6;

    //- DNA double helix vertices: two intertwined sine waves with 180° phase offset
    //- Height: 0 to 252, centered at x=21 when translated
    GLint vertices[NUM_VERTICES][3] = {
      { 10,   0,   0}, {-10,   0,   0}, {  7,  10,   7}, { -7,  10,  -7}, 
      { -1,  19,  10}, {  1,  19, -10}, { -8,  29,   6}, {  8,  29,  -6}, 
      {-10,  39,  -2}, { 10,  39,   2}, { -5,  48,  -9}, {  5,  48,   9}, 
      {  4,  58,  -9}, { -4,  58,   9}, {  9,  68,  -4}, { -9,  68,   4}, 
      {  9,  78,   5}, { -9,  78,  -5}, {  2,  87,  10}, { -2,  87, -10}, 
      { -6,  97,   8}, {  6,  97,  -8}, {-10, 107,   1}, { 10, 107,  -1}, 
      { -7, 116,  -7}, {  7, 116,   7}, {  0, 126, -10}, {  0, 126,  10}, 
      {  7, 136,  -7}, { -7, 136,   7}, { 10, 145,   1}, {-10, 145,  -1}, 
      {  6, 155,   8}, { -6, 155,  -8}, { -2, 165,  10}, {  2, 165, -10}, 
      { -9, 174,   5}, {  9, 174,  -5}, { -9, 184,  -4}, {  9, 184,   4}, 
      { -4, 194,  -9}, {  4, 194,   9}, {  5, 204,  -9}, { -5, 204,   9}, 
      { 10, 213,  -2}, {-10, 213,   2}, {  8, 223,   6}, { -8, 223,  -6}, 
      {  1, 233,  10}, { -1, 233, -10}, { -7, 242,   7}, {  7, 242,  -7}, 
      {-10, 252,   0}, { 10, 252,   0}
    };

    GLfloat vertexColors[NUM_VERTICES][3] = {
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1},
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
    };

    void draw() {
      // Draw DNA helix vertices as points
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

    // Rotate slowly only on Y-axis
    glRotatef(rotateAngle, 0.0, 1.0, 0.0);

    DNAHelix::draw();
    rotateAngle += 1;  // Slower rotation
    
    GLdouble retX, retY, retZ;

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

    //- Project all DNA helix vertices to window coordinates using gluProject
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble projectedCoords[DNAHelix::NUM_VERTICES][3];
    
    for (int i = 0; i < DNAHelix::NUM_VERTICES; i++) {
        gluProject(
            (GLdouble)DNAHelix::vertices[i][0], 
            (GLdouble)DNAHelix::vertices[i][1], 
            (GLdouble)DNAHelix::vertices[i][2],
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
  // Position camera to view the helix centered at x=21, y=126 (middle of 0-252)
  // Camera looks from front, centered on the helix
  gluLookAt(21.0, 126.0, 80.0,    // Eye position (in front, centered)
            21.0, 126.0, 0.0,      // Look at center of helix
            0.0, 1.0, 0.0);        // Up vector
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 40.0);
  glMatrixMode(GL_MODELVIEW);
}

void init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(42, 252);
  glutCreateWindow("Precalculated DNA Helix C64 Demo");
  //glutFullScreen();
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
