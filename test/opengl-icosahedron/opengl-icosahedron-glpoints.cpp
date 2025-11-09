//- OpenGL Icosahedron using GL_POINTS - 12 Vertices Only
//- A rotating icosahedron rendered as points with integer coordinates

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>

using namespace std;

static const int FPS = 60;
static const int TOTAL_FRAMES = 180;
static int frameCounter = 0;

// Rotation angles for animation
static float rotateX = 0.0f;
static float rotateY = 0.0f;
static float rotateZ = 0.0f;

namespace Icosahedron {
    // 12 vertices of an icosahedron with INTEGER coordinates
    // Based on the golden ratio positions, scaled and rounded to integers
    // Original coordinates use PHI = (1 + sqrt(5)) / 2 ≈ 1.618
    // We scale by a factor to get reasonable integer values
    // Using a scale factor of ~62 gives good integer approximations:
    // PHI * 62 ≈ 100, 1 * 62 = 62, 0 * 62 = 0
    const int NUM_VERTICES = 12;
    const int SCALE = 62;  // Scale factor for conversion to integer
    
    GLint vertices[NUM_VERTICES][3] = {
        {-62,  100,  0},   // 0:  (-1,  PHI,  0)
        { 62,  100,  0},   // 1:  ( 1,  PHI,  0)
        {-62, -100,  0},   // 2:  (-1, -PHI,  0)
        { 62, -100,  0},   // 3:  ( 1, -PHI,  0)
        {  0,  -62, 100},  // 4:  ( 0,  -1,  PHI)
        {  0,   62, 100},  // 5:  ( 0,   1,  PHI)
        {  0,  -62,-100},  // 6:  ( 0,  -1, -PHI)
        {  0,   62,-100},  // 7:  ( 0,   1, -PHI)
        {100,    0, -62},  // 8:  ( PHI,  0,  -1)
        {100,    0,  62},  // 9:  ( PHI,  0,   1)
        {-100,   0, -62},  // 10: (-PHI,  0,  -1)
        {-100,   0,  62}   // 11: (-PHI,  0,   1)
    };
    
    void draw() {
        // Set point size for better visibility
        glPointSize(8.0f);
        
        // Set point color (bright cyan-blue)
        glColor3f(0.3f, 0.7f, 1.0f);
        
        // Draw all 12 vertices as points
        glBegin(GL_POINTS);
        for (int i = 0; i < NUM_VERTICES; i++) {
            glVertex3i(vertices[i][0], vertices[i][1], vertices[i][2]);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Position camera
    gluLookAt(0.0, 0.0, 500.0,   // Eye position (moved back for integer scale)
              0.0, 0.0, 0.0,      // Look at center
              0.0, 1.0, 0.0);     // Up vector

    // Apply rotations
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // Draw the icosahedron as points
    Icosahedron::draw();

    glutSwapBuffers();
}

void timer(int value) {
    // Update frame counter
    frameCounter++;
    
    // Calculate rotation for complete 360-degree rotation over 180 frames
    float progress = (float)frameCounter / (float)TOTAL_FRAMES;
    
    // Each axis completes a full 360-degree rotation in 180 frames
    rotateX = progress * 360.0f;
    rotateY = progress * 360.0f;
    rotateZ = progress * 360.0f;
    
    // Reset after 180 frames to loop the animation
    if (frameCounter >= TOTAL_FRAMES) {
        frameCounter = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Set background color (black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Enable point smoothing for nicer looking points
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Icosahedron - GL_POINTS Mode with Integer Coordinates");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    cout << "OpenGL Icosahedron GL_POINTS Demo" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Rendering mode: GL_POINTS" << endl;
    cout << "Vertices: " << Icosahedron::NUM_VERTICES << " (integer coordinates)" << endl;
    cout << "Total animation frames: " << TOTAL_FRAMES << endl;
    cout << "Frame rate: " << FPS << " FPS" << endl;
    cout << "Animation duration: " << (float)TOTAL_FRAMES / (float)FPS << " seconds" << endl;
    cout << endl;
    cout << "Watch the icosahedron vertices rotate on all three axes!" << endl;
    
    glutMainLoop();
    return 0;
}
