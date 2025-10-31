// 3D Cube Animation Demo
// A visually appealing rotating cube with smooth animations and colorful effects

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>

using namespace std;

static const int FPS = 60;
static float rotateX = 0.0f;
static float rotateY = 0.0f;
static float rotateZ = 0.0f;
static float time_counter = 0.0f;

// Cube vertices
namespace Cube {
    const int NUM_VERTICES = 8;
    const int NUM_FACES = 6;

    GLfloat vertices[NUM_VERTICES][3] = {
        {-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, {-1.0,  1.0, -1.0}, {-1.0,  1.0,  1.0},
        { 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, { 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0}
    };

    GLint faces[NUM_FACES][4] = {
        {1, 5, 7, 3}, // Front face
        {5, 4, 6, 7}, // Right face
        {4, 0, 2, 6}, // Back face
        {3, 7, 6, 2}, // Top face
        {0, 1, 3, 2}, // Left face
        {0, 4, 5, 1}  // Bottom face
    };

    // Dynamic colors that will change over time
    GLfloat faceColors[NUM_FACES][3];

    void updateColors(float time) {
        // Generate beautiful, smoothly transitioning colors for each face
        for (int i = 0; i < NUM_FACES; i++) {
            float phase = time + i * 1.047f; // 60 degrees phase shift
            faceColors[i][0] = 0.5f + 0.5f * sin(phase);           // Red
            faceColors[i][1] = 0.5f + 0.5f * sin(phase + 2.094f);  // Green (120° shift)
            faceColors[i][2] = 0.5f + 0.5f * sin(phase + 4.189f);  // Blue (240° shift)
        }
    }

    void draw() {
        glBegin(GL_QUADS);
        for (int i = 0; i < NUM_FACES; i++) {
            // Set color for this face
            glColor3fv(faceColors[i]);
            
            // Draw the face
            for (int j = 0; j < 4; j++) {
                glVertex3fv(vertices[faces[i][j]]);
            }
        }
        glEnd();

        // Draw edges in white for better definition
        glLineWidth(2.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        
        // Define edges
        int edges[12][2] = {
            {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Left face edges
            {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Right face edges
            {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting edges
        };
        
        for (int i = 0; i < 12; i++) {
            glVertex3fv(vertices[edges[i][0]]);
            glVertex3fv(vertices[edges[i][1]]);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Position camera
    gluLookAt(0.0, 0.0, 6.0,  // Eye position
              0.0, 0.0, 0.0,  // Look at center
              0.0, 1.0, 0.0); // Up vector

    // Apply rotations with smooth animation
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // Draw the cube
    Cube::draw();

    glutSwapBuffers();
}

void timer(int value) {
    // Update rotation angles with smooth animation
    time_counter += 0.02f;
    
    // Vary rotation speeds using sine waves for organic movement
    rotateX += 0.8f + 0.5f * sin(time_counter * 0.5f);
    rotateY += 1.2f + 0.3f * sin(time_counter * 0.7f);
    rotateZ += 0.4f + 0.2f * sin(time_counter * 0.3f);

    // Update cube colors
    Cube::updateColors(time_counter);

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    
    // Enable blending for smoother colors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize colors
    Cube::updateColors(0.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Cube Animation - Colorful Rotating Cube");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    cout << "3D Cube Animation Demo" << endl;
    cout << "----------------------" << endl;
    cout << "Watch the colorful cube rotate smoothly!" << endl;
    
    glutMainLoop();
    return 0;
}
