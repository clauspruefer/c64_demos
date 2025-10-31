//- OpenGL 3D Morphing Spheres with TWF Texture
//- Three spheres of different sizes morphing into each other

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

static const int FPS = 60;
static GLfloat rotateAngle = 0;
static GLuint textureID = 0;

//- Sphere Namespace
namespace Sphere {
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;  // Normal
        float u, v;         // Texture coordinates
    };
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    
    // Generate sphere geometry
    void generate(int segments, int rings) {
        vertices.clear();
        indices.clear();
        
        for (int ring = 0; ring <= rings; ring++) {
            float theta = M_PI * ring / rings;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            
            for (int segment = 0; segment <= segments; segment++) {
                float phi = 2.0f * M_PI * segment / segments;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);
                
                Vertex v;
                // Unit sphere coordinates
                v.x = cosPhi * sinTheta;
                v.y = cosTheta;
                v.z = sinPhi * sinTheta;
                
                // Normal is same as position for unit sphere
                v.nx = v.x;
                v.ny = v.y;
                v.nz = v.z;
                
                // Texture coordinates
                v.u = (float)segment / segments;
                v.v = (float)ring / rings;
                
                vertices.push_back(v);
            }
        }
        
        // Generate indices
        for (int ring = 0; ring < rings; ring++) {
            for (int segment = 0; segment < segments; segment++) {
                int first = ring * (segments + 1) + segment;
                int second = first + segments + 1;
                
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);
                
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }
    
    // Draw sphere with morphing effect
    void draw(float radius, float yOffset, bool useTexture, float morphFactor = 0.0f, float nextRadius = 0.0f, float nextYOffset = 0.0f) {
        if (useTexture) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);
        }
        
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < indices.size(); i++) {
            unsigned int idx = indices[i];
            const Vertex& v = vertices[idx];
            
            // Apply morphing effect - blend between sphere positions
            float morphedRadius = radius;
            float morphedY = yOffset;
            
            // Morph factor affects the blending between spheres
            if (morphFactor > 0.0f && nextRadius > 0.0f) {
                // Calculate distance from equator
                float verticalPos = v.y;  // -1 to 1
                
                // Create smooth transition zone
                float transitionZone = 0.0f;
                if (nextYOffset > yOffset) {
                    // Morphing upward (current sphere morphs toward top)
                    if (verticalPos > 0.3f) {
                        transitionZone = (verticalPos - 0.3f) / 0.7f;  // 0 to 1
                    }
                } else {
                    // Morphing downward (current sphere morphs toward bottom)
                    if (verticalPos < -0.3f) {
                        transitionZone = -(verticalPos + 0.3f) / 0.7f;  // 0 to 1
                    }
                }
                
                transitionZone = fmax(0.0f, fmin(1.0f, transitionZone));
                float blend = transitionZone * morphFactor;
                
                // Blend radius and position
                morphedRadius = radius * (1.0f - blend) + nextRadius * blend;
                float targetY = yOffset + (verticalPos > 0 ? radius : -radius) * verticalPos;
                float nextTargetY = nextYOffset + (verticalPos > 0 ? nextRadius : -nextRadius) * verticalPos;
                morphedY = targetY * (1.0f - blend) + nextTargetY * blend;
            }
            
            // Calculate final position
            float px = v.x * morphedRadius;
            float py = morphedY;
            float pz = v.z * morphedRadius;
            
            // Set color based on sphere position
            if (useTexture) {
                glColor3f(1.0f, 1.0f, 1.0f);  // White for textured sphere
            } else if (yOffset > 1.5f) {
                glColor3f(0.9f, 0.3f, 0.3f);  // Reddish for top sphere
            } else if (yOffset < -1.5f) {
                glColor3f(0.3f, 0.4f, 0.9f);  // Blueish for bottom sphere
            } else {
                glColor3f(0.4f, 0.9f, 0.4f);  // Greenish for middle sphere
            }
            
            glNormal3f(v.nx, v.ny, v.nz);
            glTexCoord2f(v.u, v.v);
            glVertex3f(px, py, pz);
        }
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    }
}

// Create procedural texture with "TWF" text
void createTexture() {
    const int texWidth = 256;
    const int texHeight = 128;
    unsigned char* texData = new unsigned char[texWidth * texHeight * 3];
    
    // Initialize with background color (dark gray)
    for (int i = 0; i < texWidth * texHeight * 3; i++) {
        texData[i] = 40;
    }
    
    // Simple bitmap font for "TWF" - each letter is defined as a pattern
    // T letter - larger and more visible
    for (int x = 30; x < 75; x++) {
        for (int y = 25; y < 40; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;     // R
            texData[idx + 1] = 255; // G
            texData[idx + 2] = 255; // B
        }
    }
    for (int x = 45; x < 60; x++) {
        for (int y = 40; y < 100; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    
    // W letter - larger and more visible
    for (int x = 85; x < 100; x++) {
        for (int y = 25; y < 100; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    for (int x = 140; x < 155; x++) {
        for (int y = 25; y < 100; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    // W middle peak
    for (int x = 112; x < 127; x++) {
        for (int y = 55; y < 100; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    // W connecting lines - diagonal strokes
    for (int i = 0; i < 30; i++) {
        // Left diagonal
        for (int t = 0; t < 8; t++) {
            int x1 = 100 + i + t;
            int y1 = 25 + i * 2 + t;
            if (x1 < texWidth && y1 < texHeight) {
                int idx1 = (y1 * texWidth + x1) * 3;
                texData[idx1] = 255;
                texData[idx1 + 1] = 255;
                texData[idx1 + 2] = 255;
            }
        }
        
        // Right diagonal
        for (int t = 0; t < 8; t++) {
            int x2 = 127 + i + t;
            int y2 = 85 - i * 2 + t;
            if (x2 < texWidth && y2 >= 25 && y2 < texHeight) {
                int idx2 = (y2 * texWidth + x2) * 3;
                texData[idx2] = 255;
                texData[idx2 + 1] = 255;
                texData[idx2 + 2] = 255;
            }
        }
    }
    
    // F letter - larger and more visible
    for (int x = 170; x < 185; x++) {
        for (int y = 25; y < 100; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    // F top bar
    for (int x = 185; x < 230; x++) {
        for (int y = 25; y < 40; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    // F middle bar
    for (int x = 185; x < 220; x++) {
        for (int y = 58; y < 73; y++) {
            int idx = (y * texWidth + x) * 3;
            texData[idx] = 255;
            texData[idx + 1] = 255;
            texData[idx + 2] = 255;
        }
    }
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    
    delete[] texData;
    
    cout << "Texture created with 'TWF' text" << endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    // Rotate everything together
    glRotatef(rotateAngle, 0.2, 1.0, 0.1);
    
    // Draw morphing spheres that blend into each other
    // Stronger morphing creates more seamless connections
    float morphStrength = 0.8f;
    
    // Sphere sizes
    float topRadius = 0.6f;
    float middleRadius = 1.0f;
    float bottomRadius = 1.4f;
    
    // Sphere positions (closer together for better morphing)
    float topY = 1.8f;
    float middleY = 0.0f;
    float bottomY = -2.0f;
    
    // Top sphere (smallest) - morphs into middle
    Sphere::draw(topRadius, topY, false, morphStrength, middleRadius, middleY);
    
    // Middle sphere (medium) - with TWF texture, morphs both ways
    Sphere::draw(middleRadius, middleY, true, morphStrength, bottomRadius, bottomY);
    
    // Bottom sphere (largest)
    Sphere::draw(bottomRadius, bottomY, false, 0.0f, 0.0f, 0.0f);
    
    // Draw smooth connecting regions between spheres
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    
    // Upper connection (between top and middle)
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 64; i++) {
        float angle = 2.0f * M_PI * i / 64;
        float cosA = cos(angle);
        float sinA = sin(angle);
        
        // Smooth color gradient from red to white
        float t = (float)i / 64;
        float r1 = 0.9f, g1 = 0.3f + t * 0.5f, b1 = 0.3f + t * 0.5f;
        
        // Upper ring (bottom of top sphere)
        float r1_ring = topRadius * 0.9f;
        float y1 = topY - topRadius * 0.5f;
        glColor4f(r1, g1, b1, 0.7f);
        glVertex3f(r1_ring * cosA, y1, r1_ring * sinA);
        
        // Lower ring (top of middle sphere) 
        float r2_ring = middleRadius * 0.85f;
        float y2 = middleY + middleRadius * 0.6f;
        glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
        glVertex3f(r2_ring * cosA, y2, r2_ring * sinA);
    }
    glEnd();
    
    // Lower connection (between middle and bottom)
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 64; i++) {
        float angle = 2.0f * M_PI * i / 64;
        float cosA = cos(angle);
        float sinA = sin(angle);
        
        // Smooth color gradient from white to blue
        float t = (float)i / 64;
        
        // Upper ring (bottom of middle sphere)
        float r1_ring = middleRadius * 0.85f;
        float y1 = middleY - middleRadius * 0.6f;
        glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
        glVertex3f(r1_ring * cosA, y1, r1_ring * sinA);
        
        // Lower ring (top of bottom sphere)
        float r2_ring = bottomRadius * 0.8f;
        float y2 = bottomY + bottomRadius * 0.5f;
        float r2 = 0.3f + t * 0.1f, g2 = 0.4f + t * 0.1f, b2 = 0.9f;
        glColor4f(r2, g2, b2, 0.7f);
        glVertex3f(r2_ring * cosA, y2, r2_ring * sinA);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
    rotateAngle += 0.5f;
    
    glPopMatrix();
    
    glutSwapBuffers();
}

void timer(int v) {
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, GLfloat(w) / GLfloat(h), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 8, 0, 0, 0, 0, 1, 0);
}

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_position[] = { 5.0, 5.0, 5.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Material properties
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Background color
    glClearColor(0.1, 0.1, 0.15, 1.0);
    
    // Generate sphere geometry
    Sphere::generate(64, 32);
    
    // Create texture
    createTexture();
    
    cout << "Morphing Spheres initialized!" << endl;
    cout << "  Vertices: " << Sphere::vertices.size() << endl;
    cout << "  Triangles: " << Sphere::indices.size() / 3 << endl;
    cout << "  3 spheres with different sizes" << endl;
    cout << "  Middle sphere textured with 'TWF'" << endl;
    cout << "  Spheres morph into each other" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Morphing Spheres with TWF Texture");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
