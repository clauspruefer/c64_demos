//- OpenGL Cylinder with Helix Texture Pattern and Scroll Text
//- Based on opengl-colored-rotating-cube.cpp
//- Renders a 3D cylinder with textures winding around in a helix pattern

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include <glm/vec3.hpp>

using namespace std;

static const int FPS = 60;
static GLfloat rotateAngle = 0.0f;

//- Cylinder Namespace
namespace Cylinder {
    
    const float RADIUS = 2.0f;
    const float HEIGHT = 8.0f;
    const int RADIAL_SEGMENTS = 32;
    const int HEIGHT_SEGMENTS = 64;
    
    // Helix parameters for texture winding
    const float HELIX_ROTATIONS = 4.0f;  // Number of full rotations from bottom to top
    const int NUM_HELIX_STRANDS = 3;     // Number of texture strands winding around
    
    struct Vertex {
        float x, y, z;
        float u, v;  // Texture coordinates
        float r, g, b;  // Color
    };
    
    vector<Vertex> vertices;
    
    // Scroll text that will be repeated in texture
    const string SCROLL_TEXT = "*** C64 DEMOS *** OPENGL CYLINDER *** HELIX TEXTURE *** ";
    
    // Generate cylinder geometry with helix texture coordinates
    void generate() {
        vertices.clear();
        
        // Generate vertices for cylinder
        for (int h = 0; h <= HEIGHT_SEGMENTS; h++) {
            float y = -HEIGHT/2.0f + (HEIGHT * h) / HEIGHT_SEGMENTS;
            float vCoord = (float)h / HEIGHT_SEGMENTS;
            
            for (int r = 0; r <= RADIAL_SEGMENTS; r++) {
                float theta = 2.0f * M_PI * r / RADIAL_SEGMENTS;
                float x = RADIUS * cos(theta);
                float z = RADIUS * sin(theta);
                float uCoord = (float)r / RADIAL_SEGMENTS;
                
                // Calculate helix texture coordinates
                // Wind texture around cylinder multiple times as we go up
                float helixU = fmod(uCoord + vCoord * HELIX_ROTATIONS, 1.0f);
                
                // Color based on helix position for visualization
                // Create colored strands
                float strandPos = fmod(helixU * NUM_HELIX_STRANDS, 1.0f);
                
                // Determine which character in scroll text to show
                int textIndex = (int)(vCoord * SCROLL_TEXT.length() * 3) % SCROLL_TEXT.length();
                char c = SCROLL_TEXT[textIndex];
                
                // Create color gradient for scroll text effect
                float brightness = 0.3f + 0.7f * strandPos;
                
                // Color the different helix strands
                int strandId = (int)(helixU * NUM_HELIX_STRANDS) % NUM_HELIX_STRANDS;
                float r_val, g_val, b_val;
                
                switch(strandId) {
                    case 0:  // Red strand
                        r_val = brightness;
                        g_val = brightness * 0.2f;
                        b_val = brightness * 0.2f;
                        break;
                    case 1:  // Green strand
                        r_val = brightness * 0.2f;
                        g_val = brightness;
                        b_val = brightness * 0.2f;
                        break;
                    case 2:  // Blue strand
                        r_val = brightness * 0.2f;
                        g_val = brightness * 0.2f;
                        b_val = brightness;
                        break;
                    default:
                        r_val = g_val = b_val = brightness;
                }
                
                // Add text pattern by modulating brightness based on character
                if (c != ' ') {
                    // Make non-space characters brighter
                    r_val = min(1.0f, r_val * 1.5f);
                    g_val = min(1.0f, g_val * 1.5f);
                    b_val = min(1.0f, b_val * 1.5f);
                }
                
                Vertex v;
                v.x = x;
                v.y = y;
                v.z = z;
                v.u = helixU;
                v.v = vCoord;
                v.r = r_val;
                v.g = g_val;
                v.b = b_val;
                
                vertices.push_back(v);
            }
        }
    }
    
    void draw() {
        // Draw cylinder as quad strips
        for (int h = 0; h < HEIGHT_SEGMENTS; h++) {
            glBegin(GL_QUAD_STRIP);
            for (int r = 0; r <= RADIAL_SEGMENTS; r++) {
                int idx1 = h * (RADIAL_SEGMENTS + 1) + r;
                int idx2 = (h + 1) * (RADIAL_SEGMENTS + 1) + r;
                
                Vertex& v1 = vertices[idx1];
                Vertex& v2 = vertices[idx2];
                
                glColor3f(v1.r, v1.g, v1.b);
                glVertex3f(v1.x, v1.y, v1.z);
                
                glColor3f(v2.r, v2.g, v2.b);
                glVertex3f(v2.x, v2.y, v2.z);
            }
            glEnd();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    // Rotate the cylinder
    glRotatef(rotateAngle, 0.0, 1.0, 0.0);
    
    Cylinder::draw();
    
    // Increment rotation angle
    rotateAngle += 1.0f;
    if (rotateAngle >= 360.0f) {
        rotateAngle -= 360.0f;
    }
    
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void timer(int v) {
    glLoadIdentity();
    // Position camera to view the cylinder
    gluLookAt(8.0, 2.0, 8.0,     // Eye position
              0.0, 0.0, 0.0,      // Look at center
              0.0, 1.0, 0.0);     // Up vector
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, GLfloat(w) / GLfloat(h), 0.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Generate cylinder geometry
    Cylinder::generate();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Cylinder with Helix Texture and Scroll Text");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
