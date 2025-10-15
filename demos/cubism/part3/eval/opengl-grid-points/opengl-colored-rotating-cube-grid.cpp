//- Taken from OpenGL examples, used for c64-Demo precalculated colored cube

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

//- Cube Namespace
namespace Cube {

    const int NUM_CUBE_VERTICES = 96;
    const int NUM_PYRAMID_APEXES = 6;

    GLint vertices[NUM_CUBE_VERTICES][3] = {
      { -8,  -2,  -2}, { -8,  -2,   2}, { -8,   2,  -2}, { -8,   2,   2},
      { -7,  -5,   0}, { -7,   0,  -5}, { -7,   0,   5}, { -7,   5,   0},
      { -6,  -5,  -3}, { -6,  -5,   3}, { -6,  -3,  -5}, { -6,  -3,   5},
      { -6,   3,  -5}, { -6,   3,   5}, { -6,   5,  -3}, { -6,   5,   3},
      { -5,  -7,   0}, { -5,  -6,  -3}, { -5,  -6,   3}, { -5,  -3,  -6},
      { -5,  -3,   6}, { -5,   0,  -7}, { -5,   0,   7}, { -5,   3,  -6},
      { -5,   3,   6}, { -5,   6,  -3}, { -5,   6,   3}, { -5,   7,   0},
      { -3,  -6,  -5}, { -3,  -6,   5}, { -3,  -5,  -6}, { -3,  -5,   6},
      { -3,   5,  -6}, { -3,   5,   6}, { -3,   6,  -5}, { -3,   6,   5},
      { -2,  -8,  -2}, { -2,  -8,   2}, { -2,  -2,  -8}, { -2,  -2,   8},
      { -2,   2,  -8}, { -2,   2,   8}, { -2,   8,  -2}, { -2,   8,   2},
      {  0,  -7,  -5}, {  0,  -7,   5}, {  0,  -5,  -7}, {  0,  -5,   7},
      {  0,   5,  -7}, {  0,   5,   7}, {  0,   7,  -5}, {  0,   7,   5},
      {  2,  -8,  -2}, {  2,  -8,   2}, {  2,  -2,  -8}, {  2,  -2,   8},
      {  2,   2,  -8}, {  2,   2,   8}, {  2,   8,  -2}, {  2,   8,   2},
      {  3,  -6,  -5}, {  3,  -6,   5}, {  3,  -5,  -6}, {  3,  -5,   6},
      {  3,   5,  -6}, {  3,   5,   6}, {  3,   6,  -5}, {  3,   6,   5},
      {  5,  -7,   0}, {  5,  -6,  -3}, {  5,  -6,   3}, {  5,  -3,  -6},
      {  5,  -3,   6}, {  5,   0,  -7}, {  5,   0,   7}, {  5,   3,  -6},
      {  5,   3,   6}, {  5,   6,  -3}, {  5,   6,   3}, {  5,   7,   0},
      {  6,  -5,  -3}, {  6,  -5,   3}, {  6,  -3,  -5}, {  6,  -3,   5},
      {  6,   3,  -5}, {  6,   3,   5}, {  6,   5,  -3}, {  6,   5,   3},
      {  7,  -5,   0}, {  7,   0,  -5}, {  7,   0,   5}, {  7,   5,   0},
      {  8,  -2,  -2}, {  8,  -2,   2}, {  8,   2,  -2}, {  8,   2,   2}
    };

    GLfloat vertexColors[NUM_CUBE_VERTICES][3] = {
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
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
    }

    void draw() {
      // Draw rhombicosidodecahedron vertices as points
      glBegin(GL_POINTS);
      for (int i = 0; i < NUM_CUBE_VERTICES; i++) {
        glColor3fv((GLfloat*)&vertexColors[i]);
        glVertex3iv((GLint*)&vertices[i]);
      }
      glEnd();
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(rotateAngle, 0.5, 0.8, 0.8);

    Cube::draw();
    rotateAngle += 2;
    
    GLdouble retX, retY, retZ;

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

    //- Project all rhombicosidodecahedron vertices to window coordinates using gluProject
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble projectedCoords[Cube::NUM_CUBE_VERTICES][3];
    
    for (int i = 0; i < Cube::NUM_CUBE_VERTICES; i++) {
        gluProject(
            (GLdouble)Cube::vertices[i][0], 
            (GLdouble)Cube::vertices[i][1], 
            (GLdouble)Cube::vertices[i][2],
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
  //static GLfloat u = 4.74;
  static GLfloat u = 4.74;
  //u += 0.03;
  glLoadIdentity();
  gluLookAt(8*cos(u), 7*cos(u)-1, 4*cos(u/3)+2, .5, .5, .5, cos(u), 1, 0);
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
  glutInitWindowSize(96, 80);
  glutCreateWindow("Precalculated Cube C64 Demo");
  //glutFullScreen();
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
