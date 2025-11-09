//- OpenGL Animated Icosahedron - Realistic Shaded 3D Model
//- A rotating icosahedron with two light sources and smooth shading

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
    // Golden ratio constant
    const float PHI = (1.0f + sqrt(5.0f)) / 2.0f;
    const float SCALE = 1.5f;  // Scale factor for better visibility
    
    // 12 vertices of an icosahedron
    const int NUM_VERTICES = 12;
    GLfloat vertices[NUM_VERTICES][3] = {
        {-1.0f,  PHI,  0.0f},  // 0
        { 1.0f,  PHI,  0.0f},  // 1
        {-1.0f, -PHI,  0.0f},  // 2
        { 1.0f, -PHI,  0.0f},  // 3
        { 0.0f, -1.0f,  PHI},  // 4
        { 0.0f,  1.0f,  PHI},  // 5
        { 0.0f, -1.0f, -PHI},  // 6
        { 0.0f,  1.0f, -PHI},  // 7
        { PHI,  0.0f, -1.0f},  // 8
        { PHI,  0.0f,  1.0f},  // 9
        {-PHI,  0.0f, -1.0f},  // 10
        {-PHI,  0.0f,  1.0f}   // 11
    };
    
    // 20 triangular faces of an icosahedron
    const int NUM_FACES = 20;
    GLint faces[NUM_FACES][3] = {
        // 5 faces around point 0
        {0, 11, 5},
        {0, 5, 1},
        {0, 1, 7},
        {0, 7, 10},
        {0, 10, 11},
        
        // 5 adjacent faces
        {1, 5, 9},
        {5, 11, 4},
        {11, 10, 2},
        {10, 7, 6},
        {7, 1, 8},
        
        // 5 faces around point 3
        {3, 9, 4},
        {3, 4, 2},
        {3, 2, 6},
        {3, 6, 8},
        {3, 8, 9},
        
        // 5 adjacent faces
        {4, 9, 5},
        {2, 4, 11},
        {6, 2, 10},
        {8, 6, 7},
        {9, 8, 1}
    };
    
    // Calculate normal for a triangle face
    void calculateNormal(int faceIndex, GLfloat normal[3]) {
        int v0 = faces[faceIndex][0];
        int v1 = faces[faceIndex][1];
        int v2 = faces[faceIndex][2];
        
        // Get vertices
        GLfloat *p0 = vertices[v0];
        GLfloat *p1 = vertices[v1];
        GLfloat *p2 = vertices[v2];
        
        // Calculate two edge vectors
        GLfloat edge1[3] = {
            (p1[0] - p0[0]) * SCALE,
            (p1[1] - p0[1]) * SCALE,
            (p1[2] - p0[2]) * SCALE
        };
        
        GLfloat edge2[3] = {
            (p2[0] - p0[0]) * SCALE,
            (p2[1] - p0[1]) * SCALE,
            (p2[2] - p0[2]) * SCALE
        };
        
        // Calculate cross product
        normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
        normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
        normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
        
        // Normalize
        GLfloat length = sqrt(normal[0] * normal[0] + 
                            normal[1] * normal[1] + 
                            normal[2] * normal[2]);
        
        if (length > 0.0f) {
            normal[0] /= length;
            normal[1] /= length;
            normal[2] /= length;
        }
    }
    
    void draw() {
        // Set material properties for the icosahedron
        GLfloat mat_ambient[] = {0.2f, 0.2f, 0.3f, 1.0f};
        GLfloat mat_diffuse[] = {0.3f, 0.5f, 0.8f, 1.0f};
        GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat mat_shininess[] = {50.0f};
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        
        // Draw each face as a filled triangle
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < NUM_FACES; i++) {
            // Calculate and set normal for this face
            GLfloat normal[3];
            calculateNormal(i, normal);
            glNormal3fv(normal);
            
            // Draw the three vertices of this triangular face
            for (int j = 0; j < 3; j++) {
                int vertexIndex = faces[i][j];
                glVertex3f(vertices[vertexIndex][0] * SCALE,
                          vertices[vertexIndex][1] * SCALE,
                          vertices[vertexIndex][2] * SCALE);
            }
        }
        glEnd();
    }
}

void setupLighting() {
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Left front light
    glEnable(GL_LIGHT1);  // Right front light
    
    // Light 0: Left front position
    GLfloat light0_position[] = {-5.0f, 3.0f, 5.0f, 1.0f};
    GLfloat light0_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light0_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    // Light 1: Right front position
    GLfloat light1_position[] = {5.0f, 3.0f, 5.0f, 1.0f};
    GLfloat light1_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat light1_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat light1_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Position camera
    gluLookAt(0.0, 0.0, 8.0,   // Eye position
              0.0, 0.0, 0.0,   // Look at center
              0.0, 1.0, 0.0);  // Up vector

    // Apply rotations
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // Draw the icosahedron
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
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Enable smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Setup lighting
    setupLighting();
    
    // Enable face culling for better performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Animated Icosahedron - Realistic Shaded");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    cout << "OpenGL Animated Icosahedron Demo" << endl;
    cout << "--------------------------------" << endl;
    cout << "Total animation frames: " << TOTAL_FRAMES << endl;
    cout << "Frame rate: " << FPS << " FPS" << endl;
    cout << "Animation duration: " << (float)TOTAL_FRAMES / (float)FPS << " seconds" << endl;
    cout << "Two light sources: Left front and Right front" << endl;
    cout << endl;
    cout << "Watch the icosahedron rotate on all three axes!" << endl;
    
    glutMainLoop();
    return 0;
}
