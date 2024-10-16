//- Taken from OpenGL examples, used for c64-Demo precalculated colored cube

#include <GL/glut.h>

#include <cmath>
#include <iostream>

using namespace std;

static const int FPS = 120;
static GLint rotateAngle = 0;

//- Cube "c" Namespace
namespace Cube {

    const int NUM_VERTICES = 8;
    const int NUM_FACES = 6;

    GLint vertices[NUM_VERTICES][3] = {
      {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
      {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

    GLint faces[NUM_FACES][4] = {
      {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6},
      {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}};

    GLfloat vertexColors[NUM_VERTICES][3] = {
      {1.0, 0.2, 0.2}, {0.2, 1.0, 0.5}, {1.0, 1.0, 0.2}, {1.0, 1.0, 0.8},
      {1.0, 0.8, 0.8}, {0.6, 1.0, 0.5}, {1.0, 0.5, 1.0}, {1.0, 1.0, 0.2}};

    void draw() {
      glBegin(GL_QUADS);
      for (int i = 0; i < NUM_FACES; i++) {
        for (int j = 0; j < 4; j++) {
          glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
          glVertex3iv((GLint*)&vertices[faces[i][j]]);
        }
      }
      glEnd();
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glRotatef(rotateAngle, 1.0, 1.0, 1.0);
    rotateAngle += 1;

    Cube::draw();

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

    glPopMatrix();
    glFlush();

    glutSwapBuffers();
}

void timer(int v) {
  static GLfloat u = 5;
  //u += 0.01;
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
  glutInitWindowSize(160, 100);
  glutCreateWindow("Precalculated Cube C64 Demo");
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
