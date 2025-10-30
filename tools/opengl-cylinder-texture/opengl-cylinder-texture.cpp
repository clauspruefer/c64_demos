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
    
    struct Vertex {
        float x, y, z;
        float u, v;  // Texture coordinates
        float r, g, b;  // Color
    };
    
    vector<Vertex> vertices;
    
    // Scroll text that will be repeated - one character per vertical position
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
                
                // Calculate helix position - wind around cylinder as we go up
                // The helix angle increases with height
                float helixAngle = uCoord + vCoord * HELIX_ROTATIONS;
                float helixU = fmod(helixAngle, 1.0f);
                
                // Determine which character in scroll text to display at this height
                // Map the vertical position to a character in the scroll text
                // Use more repetitions to make letters more visible
                int charIndex = (int)(vCoord * SCROLL_TEXT.length() * 4) % SCROLL_TEXT.length();
                char currentChar = SCROLL_TEXT[charIndex];
                
                // Determine if this vertex should display the text
                // Text appears along the helix path - create a rope-like band
                // The text "rope" follows the helix, appearing when helixU is near 0.0
                float textBandWidth = 0.2f;  // Width of the text band (rope width)
                bool isOnTextPath = (helixU < textBandWidth) || (helixU > (1.0f - textBandWidth));
                
                // Create brightness based on whether we're on the text path and the character
                float brightness;
                if (isOnTextPath) {
                    // On the helix rope path
                    if (currentChar != ' ') {
                        // Bright white for text characters
                        brightness = 1.0f;
                    } else {
                        // Medium gray for spaces (still visible as part of the rope)
                        brightness = 0.4f;
                    }
                } else {
                    // Background - very dark
                    brightness = 0.05f;
                }
                
                // Use white/grayscale only (no RGB coloring)
                float r_val = brightness;
                float g_val = brightness;
                float b_val = brightness;
                
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
