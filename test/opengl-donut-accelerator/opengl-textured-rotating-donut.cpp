//- OpenGL Textured Rotating Donut - Particle Accelerator Ring
//- Based on opengl-colored-rotating-cube.cpp but with torus geometry and textures

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <vector>

#include <glm/vec3.hpp>

using namespace std;

static const int FPS = 60;
static GLint rotateAngle = 0;

//- Torus (Donut) Namespace
namespace Torus {
    // Torus parameters - adjusted for better donut appearance
    const float MAJOR_RADIUS = 2.5f;  // Distance from center to tube center
    const float MINOR_RADIUS = 0.8f;  // Tube radius (thicker for better donut look)
    const int MAJOR_SEGMENTS = 96;     // Segments around the major circle (smoother)
    const int MINOR_SEGMENTS = 48;     // Segments around the tube (smoother)
    
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;  // Normal
        float u, v;         // Texture coordinates
    };
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    
    // Generate torus geometry
    void generate() {
        vertices.clear();
        indices.clear();
        
        for (int i = 0; i <= MAJOR_SEGMENTS; i++) {
            float theta = 2.0f * M_PI * i / MAJOR_SEGMENTS;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            
            for (int j = 0; j <= MINOR_SEGMENTS; j++) {
                float phi = 2.0f * M_PI * j / MINOR_SEGMENTS;
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);
                
                Vertex v;
                // Position
                v.x = (MAJOR_RADIUS + MINOR_RADIUS * cosPhi) * cosTheta;
                v.y = (MAJOR_RADIUS + MINOR_RADIUS * cosPhi) * sinTheta;
                v.z = MINOR_RADIUS * sinPhi;
                
                // Normal
                v.nx = cosPhi * cosTheta;
                v.ny = cosPhi * sinTheta;
                v.nz = sinPhi;
                
                // Texture coordinates
                v.u = (float)i / MAJOR_SEGMENTS;
                v.v = (float)j / MINOR_SEGMENTS;
                
                vertices.push_back(v);
            }
        }
        
        // Generate indices
        for (int i = 0; i < MAJOR_SEGMENTS; i++) {
            for (int j = 0; j < MINOR_SEGMENTS; j++) {
                int first = i * (MINOR_SEGMENTS + 1) + j;
                int second = first + MINOR_SEGMENTS + 1;
                
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);
                
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }
    
    // Draw the torus with advanced procedural texturing
    void draw() {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < indices.size(); i++) {
            unsigned int idx = indices[i];
            const Vertex& v = vertices[idx];
            
            float u = v.u;
            float vCoord = v.v;
            
            // Base colors
            float r = 0.0f, g = 0.0f, b = 0.0f;
            
            // Determine position type based on v coordinate (around tube)
            // Top (outer): v ~0 or ~1, Bottom (inner): v ~0.5
            float vNormalized = fmod(vCoord + 0.25f, 1.0f); // Rotate to make top at 0
            
            // Create accelerator segments (32 sections around the ring)
            float segmentU = u * 32.0f;
            float segmentId = floor(segmentU);
            float segmentPos = fmod(segmentU, 1.0f);
            
            // Magnet sections (alternating red/blue dipole magnets)
            bool isMagnetSection = (fmod(segmentId, 4.0f) < 2.0f);
            bool isRedMagnet = (fmod(segmentId, 4.0f) < 1.0f);
            
            // Outer surface (top of donut) - Main accelerator pipe
            if (vNormalized < 0.15f || vNormalized > 0.85f) {
                // Stainless steel beam pipe
                r = 0.7f; g = 0.72f; b = 0.75f;
                
                // Add panel lines
                if (segmentPos < 0.05f || segmentPos > 0.95f) {
                    r *= 0.6f; g *= 0.6f; b *= 0.6f;
                }
                
                // Add rivets/bolts
                if (fmod(u * 128.0f, 1.0f) < 0.05f && (vNormalized < 0.05f || vNormalized > 0.95f)) {
                    r = 0.5f; g = 0.5f; b = 0.55f;
                }
            }
            // Inner surface (bottom of donut) - Beam tube with energy glow
            else if (vNormalized > 0.45f && vNormalized < 0.55f) {
                // Bright energy beam glow (like particle beam)
                r = 0.3f; g = 0.5f; b = 1.0f;
                
                // Pulsing effect
                float pulse = 0.3f + 0.7f * (0.5f + 0.5f * sin(u * 64.0f));
                r *= pulse; g *= pulse; b *= pulse;
                
                // Make it brighter
                r += 0.4f; g += 0.6f; b += 0.8f;
            }
            // Side surfaces - Magnet systems and equipment
            else if (vNormalized >= 0.15f && vNormalized <= 0.45f) {
                // Top side - Magnet coils and cryogenic systems
                if (isMagnetSection) {
                    // Dipole magnets (alternating polarity)
                    if (isRedMagnet) {
                        r = 0.8f; g = 0.2f; b = 0.2f; // Red magnet
                    } else {
                        r = 0.2f; g = 0.3f; b = 0.9f; // Blue magnet
                    }
                    
                    // Add copper coil texture
                    float coilPattern = fmod(vNormalized * 64.0f, 1.0f);
                    if (coilPattern < 0.15f) {
                        r = 0.72f; g = 0.45f; b = 0.20f; // Copper color
                    }
                } else {
                    // Detector/monitoring sections (white/gray)
                    r = 0.85f; g = 0.85f; b = 0.88f;
                    
                    // Add sensor grid pattern
                    if (fmod(vNormalized * 32.0f, 1.0f) < 0.1f || fmod(segmentU * 4.0f, 1.0f) < 0.1f) {
                        r = 0.3f; g = 0.35f; b = 0.4f;
                    }
                }
                
                // Add cooling pipes (blue horizontal lines)
                if (fmod(vNormalized * 16.0f, 1.0f) < 0.08f) {
                    r = 0.3f; g = 0.6f; b = 0.8f;
                }
            }
            // Bottom side - Control terminals and cable trays
            else {
                // Cable trays (dark gray with cables)
                r = 0.25f; g = 0.25f; b = 0.28f;
                
                // Individual cables (various colors)
                float cableV = fmod(vNormalized * 96.0f, 1.0f);
                float cableU = fmod(u * 256.0f, 1.0f);
                if (cableV < 0.05f) {
                    // Power cables (thick, various colors)
                    float cableType = floor(fmod(u * 256.0f, 6.0f));
                    if (cableType < 1.0f) { r = 0.9f; g = 0.1f; b = 0.1f; } // Red
                    else if (cableType < 2.0f) { r = 0.1f; g = 0.1f; b = 0.9f; } // Blue
                    else if (cableType < 3.0f) { r = 0.9f; g = 0.9f; b = 0.1f; } // Yellow
                    else if (cableType < 4.0f) { r = 0.1f; g = 0.8f; b = 0.1f; } // Green
                    else if (cableType < 5.0f) { r = 0.1f; g = 0.1f; b = 0.1f; } // Black
                    else { r = 0.9f; g = 0.9f; b = 0.9f; } // White
                }
                
                // Terminal/control boxes (every 8th segment)
                if (fmod(segmentId, 8.0f) < 1.0f && segmentPos > 0.2f && segmentPos < 0.8f) {
                    // Equipment rack (beige/gray)
                    r = 0.65f; g = 0.62f; b = 0.58f;
                    
                    // LED indicators
                    float ledV = vNormalized - 0.55f;
                    if (ledV > 0.0f && ledV < 0.15f) {
                        float ledPattern = fmod(ledV * 32.0f, 1.0f);
                        if (ledPattern < 0.3f) {
                            // Green status LEDs (most common)
                            r = 0.1f; g = 0.9f; b = 0.1f;
                        } else if (ledPattern < 0.35f) {
                            // Red warning LEDs (occasional)
                            r = 0.9f; g = 0.1f; b = 0.1f;
                        }
                    }
                    
                    // Display screens (LCD panels)
                    if (vNormalized > 0.65f && vNormalized < 0.75f && segmentPos > 0.3f && segmentPos < 0.7f) {
                        // Cyan LCD display
                        r = 0.1f; g = 0.3f; b = 0.4f;
                        
                        // Display text effect (horizontal lines)
                        if (fmod(vNormalized * 128.0f, 1.0f) < 0.5f) {
                            r = 0.2f; g = 0.8f; b = 0.9f;
                        }
                    }
                    
                    // Panel edges/borders
                    if (segmentPos < 0.25f || segmentPos > 0.75f || 
                        vNormalized < 0.57f || vNormalized > 0.78f) {
                        r *= 0.4f; g *= 0.4f; b *= 0.4f;
                    }
                }
            }
            
            // Add general weathering and detail
            // Random slight variations for realism
            float noise = sin(u * 427.123f + vCoord * 731.456f) * 0.03f;
            r = fmax(0.0f, fmin(1.0f, r + noise));
            g = fmax(0.0f, fmin(1.0f, g + noise));
            b = fmax(0.0f, fmin(1.0f, b + noise));
            
            glColor3f(r, g, b);
            glNormal3f(v.nx, v.ny, v.nz);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
    
    // Rotate the donut
    glRotatef(rotateAngle, 0.3, 0.8, 0.1);
    
    Torus::draw();
    rotateAngle += 1;
    
    glPopMatrix();
    
    glutSwapBuffers();
}

void timer(int v) {
    static GLfloat u = 4.74;
    u += 0.015;  // Slower rotation for better viewing
    glLoadIdentity();
    
    // Position camera to view the donut from various angles
    // Adjusted for better 3D perspective
    float camDist = 9.0f;
    gluLookAt(
        camDist * cos(u), 
        4.5f * sin(u * 0.7f), 
        camDist * sin(u) * 0.6f + 2.0f,  // Eye position (orbiting)
        0.0, 0.0, 0.0,                    // Look at center
        0, 1, 0                            // Up vector
    );
    
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
    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable lighting for better visualization
    glEnable(GL_LIGHTING);
    
    // Main light (key light from above-right)
    glEnable(GL_LIGHT0);
    GLfloat light0_position[] = { 8.0, 6.0, 4.0, 1.0 };
    GLfloat light0_ambient[] = { 0.2, 0.2, 0.25, 1.0 };
    GLfloat light0_diffuse[] = { 0.9, 0.9, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    // Fill light (from left side, softer)
    glEnable(GL_LIGHT1);
    GLfloat light1_position[] = { -5.0, 2.0, 3.0, 1.0 };
    GLfloat light1_ambient[] = { 0.1, 0.1, 0.15, 1.0 };
    GLfloat light1_diffuse[] = { 0.5, 0.5, 0.6, 1.0 };
    GLfloat light1_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    
    // Rim light (from behind, creates depth)
    glEnable(GL_LIGHT2);
    GLfloat light2_position[] = { 0.0, -3.0, -5.0, 1.0 };
    GLfloat light2_ambient[] = { 0.05, 0.05, 0.1, 1.0 };
    GLfloat light2_diffuse[] = { 0.4, 0.5, 0.7, 1.0 };
    GLfloat light2_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    
    // Material properties - higher shininess for metallic look
    GLfloat mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_shininess[] = { 80.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Enable smooth shading for better appearance
    glShadeModel(GL_SMOOTH);
    
    // Set background color (dark tunnel/facility)
    glClearColor(0.08, 0.08, 0.12, 1.0);
    
    // Generate torus geometry
    Torus::generate();
    
    cout << "Particle Accelerator Donut initialized!" << endl;
    cout << "Vertices: " << Torus::vertices.size() << endl;
    cout << "Triangles: " << Torus::indices.size() / 3 << endl;
    cout << "Features: Magnets, Detectors, Terminals, Beam Pipe, Cryogenics" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modern Particle Accelerator Ring - LHC Style");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
