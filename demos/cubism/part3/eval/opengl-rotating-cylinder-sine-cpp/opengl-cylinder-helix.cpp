//- Taken from OpenGL examples, used for c64-Demo precalculated double helix with intertwined ropes

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>

#include <glm/vec3.hpp>

using namespace std;

static const int FPS = 60;
static GLfloat rotateAngle = 0.0f;
static GLint rotateAngle2 = 0;

namespace DoubleHelix {

    const int NUM_VERTICES = 32;
    GLint vertices[NUM_VERTICES][3];
    GLfloat vertexColors[NUM_VERTICES][3];

    void draw() {
      // Draw double helix vertices as points (stay in pixel draw mode - do NOT connect)
      glBegin(GL_POINTS);
      for (int i = 0; i < NUM_VERTICES; i++) {
        glColor3fv((GLfloat*)&vertexColors[i]);
        glVertex3iv((GLint*)&vertices[i]);
      }
      glEnd();
    }
}

void generateDoubleHelix() {

  const float radius = 2.0f;
  const float height = 30.0f;
  const int pointsPerHelix = DoubleHelix::NUM_VERTICES/2;

  int coordIndex = 0;

  for (int i = 0; i < pointsPerHelix; i++) {

      float t = (float)i / (float)(pointsPerHelix - 1);
      float angle = t * 4.0f * M_PI;

      DoubleHelix::vertices[coordIndex][0] = radius * cos(angle) *2;
      DoubleHelix::vertices[coordIndex][1] = radius * sin(angle) *2;
      DoubleHelix::vertices[coordIndex][2] = -height/2 + height * t *2;

      DoubleHelix::vertexColors[coordIndex][0] = 1;
      DoubleHelix::vertexColors[coordIndex][1] = 1;
      DoubleHelix::vertexColors[coordIndex][2] = 1;

      coordIndex += 1;
  }

  for (int i = 0; i < pointsPerHelix; i++) {

      float t = (float)i / (float)(pointsPerHelix - 1);
      float angle = t * 4.0f * M_PI + M_PI;

      DoubleHelix::vertices[coordIndex][0] = radius * cos(angle) *2;
      DoubleHelix::vertices[coordIndex][1] = radius * sin(angle) *2;
      DoubleHelix::vertices[coordIndex][2] = -height/2 + height * t *2;

      DoubleHelix::vertexColors[coordIndex][0] = 1;
      DoubleHelix::vertexColors[coordIndex][1] = 1;
      DoubleHelix::vertexColors[coordIndex][2] = 1;

      coordIndex += 1;
  }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(rotateAngle, 0.0, 1.0, rotateAngle);

    DoubleHelix::draw();

    // Increment by 11.25 degrees per frame for 32-frame loop (360/32 = 11.25)
    rotateAngle += 2.25f;

    /*
    rotateAngle += 11.25f;
    if (rotateAngle >= 360.0f) rotateAngle -= 360.0f;
    */

    GLdouble retX, retY, retZ;

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    //cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

    //- Project all double helix vertices to window coordinates using gluProject
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble projectedCoords[DoubleHelix::NUM_VERTICES][3];

    for (int i = 0; i < DoubleHelix::NUM_VERTICES; i++) {
        gluProject(
            (GLdouble)DoubleHelix::vertices[i][0],
            (GLdouble)DoubleHelix::vertices[i][1],
            (GLdouble)DoubleHelix::vertices[i][2],
            modelview, 
            projection,
            viewport,
            &projectedCoords[i][0], 
            &projectedCoords[i][1], 
            &projectedCoords[i][2]
        );
    }

    glPopMatrix();
    glFlush();

    glutSwapBuffers();
}

void timer(int v) {
  glLoadIdentity();
  // Position camera with angled view: cylinder farther in z at top, closer at bottom
  // Eye positioned at an angle to create perspective effect
  // Looking slightly downward from above to see the helix structure better
  gluLookAt(0.0, 35.0, 55.0,     // Eye position (slightly above, far back)
            0.0, 0.0, 0.0,       // Look at center of double helix
            0.0, 4.0, 0.0);      // Up vector
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
  generateDoubleHelix();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  //glutInitWindowSize(420, 2520);
  glutInitWindowSize(42, 252);
  glutCreateWindow("Precalculated Double Helix C64 Demo");
  //glutFullScreen();
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
