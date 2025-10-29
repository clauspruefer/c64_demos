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

//- Double Helix Namespace
namespace DoubleHelix {

    const int NUM_VERTICES = 18;
    const int NUM_PYRAMID_APEXES = 6;

    //- Double helix vertices: two intertwined ropes wrapping around cylinder
    //- 9 vertices per strand, interleaved (strand1[0], strand2[0], strand1[1], strand2[1], ...)
    //- Improved with larger radius (~8 units) and height (30 units) for better visibility
    //- Reduced to 18 vertices total for C64 demo performance
    GLint vertices[NUM_VERTICES][3] = {
      {  8,  15,   0}, { -8,  15,   0}, 
      { -7,  11,  -5}, {  7,  11,   5}, 
      {  6,   8,   4}, { -6,   8,  -4}, 
      { -5,   4,  -8}, {  5,   4,   8}, 
      { -3,   0,   7}, {  3,   0,  -7}, 
      {  2,  -4,  -9}, { -2,  -4,   9}, 
      { -4,  -8,   6}, {  4,  -8,  -6}, 
      {  8, -11,  -1}, { -8, -11,   1}, 
      { -8, -15,   0}, {  8, -15,   0}
    };

    GLfloat vertexColors[NUM_VERTICES][3] = {
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, 
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, 
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
    };

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

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(rotateAngle, 0.0, 1.0, 0.0);

    DoubleHelix::draw();
    // Increment by 11.25 degrees per frame for 32-frame loop (360/32 = 11.25)
    rotateAngle += 11.25f;
    if (rotateAngle >= 360.0f) rotateAngle -= 360.0f;
    
    GLdouble retX, retY, retZ;

    //- Get Pixel Data
    GLubyte pixels[3];
    glReadPixels(50, 50, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
    cout << "Pixel R:" << static_cast<int>(pixels[0]) << " G:"  << static_cast<int>(pixels[1]) << " B:" << static_cast<int>(pixels[2]) << endl;

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
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(420, 2520);
  glutCreateWindow("Precalculated Double Helix C64 Demo");
  //glutFullScreen();
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}
