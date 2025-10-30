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
    
    // Generate torus geometry with 3D surface details
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
                
                // UV coordinates for procedural displacement
                float u = (float)i / MAJOR_SEGMENTS;
                float v = (float)j / MINOR_SEGMENTS;
                
                // Calculate displacement based on surface features
                float displacement = 0.0f;
                
                // Normalize v to determine position around tube
                float vNormalized = fmod(v + 0.25f, 1.0f);
                
                // Segment information
                float segmentU = u * 32.0f;
                float segmentId = floor(segmentU);
                float segmentPos = fmod(segmentU, 1.0f);
                
                // Add 3D equipment boxes (terminals) - they protrude from surface
                if (fmod(segmentId, 8.0f) < 1.0f && segmentPos > 0.2f && segmentPos < 0.8f) {
                    // Bottom side: equipment terminals that stick out
                    if (vNormalized > 0.55f && vNormalized < 0.80f) {
                        // Box-like protrusion
                        float boxDepth = 0.15f;
                        float edgeFactor = 1.0f;
                        
                        // Create beveled edges
                        if (segmentPos < 0.25f || segmentPos > 0.75f) {
                            edgeFactor = 0.5f + 0.5f * sin((segmentPos - 0.2f) / 0.6f * M_PI);
                        }
                        if (vNormalized < 0.58f || vNormalized > 0.77f) {
                            edgeFactor *= 0.5f + 0.5f * sin((vNormalized - 0.55f) / 0.25f * M_PI);
                        }
                        
                        displacement += boxDepth * edgeFactor;
                    }
                }
                
                // Add magnet coil windings - circular ridges
                if (vNormalized >= 0.15f && vNormalized <= 0.45f) {
                    bool isMagnetSection = (fmod(segmentId, 4.0f) < 2.0f);
                    if (isMagnetSection) {
                        // Copper coil ridges
                        float coilPattern = sin(vNormalized * 64.0f * M_PI);
                        displacement += 0.02f * coilPattern;
                        
                        // Support structure bumps every 4 segments
                        if (fmod(segmentU * 4.0f, 1.0f) < 0.1f) {
                            displacement += 0.05f;
                        }
                    }
                }
                
                // Add cooling pipe ridges (run horizontally around ring)
                if (vNormalized >= 0.20f && vNormalized <= 0.40f) {
                    float pipePattern = fmod(vNormalized * 16.0f, 1.0f);
                    if (pipePattern < 0.15f) {
                        // Cylindrical pipe cross-section
                        float pipeCurve = sin(pipePattern / 0.15f * M_PI);
                        displacement += 0.04f * pipeCurve;
                    }
                }
                
                // Add detector housing (rectangular boxes)
                bool isDetectorSection = (fmod(segmentId, 4.0f) >= 2.0f);
                if (isDetectorSection && vNormalized >= 0.15f && vNormalized <= 0.45f) {
                    // Detector box protrusions
                    if (segmentPos > 0.15f && segmentPos < 0.85f) {
                        float boxProfile = 1.0f;
                        if (segmentPos < 0.20f || segmentPos > 0.80f) {
                            boxProfile = sin((segmentPos - 0.15f) / 0.7f * M_PI);
                        }
                        displacement += 0.08f * boxProfile;
                    }
                }
                
                // Add cable tray ridges and individual cable bumps
                if (vNormalized >= 0.65f && vNormalized <= 0.85f) {
                    // Cable tray walls
                    if (vNormalized < 0.68f || vNormalized > 0.82f) {
                        displacement += 0.03f;
                    }
                    
                    // Individual cable bumps
                    float cableV = vNormalized * 96.0f;
                    float cableU = u * 256.0f;
                    if (fmod(cableV, 1.0f) < 0.3f) {
                        float cableRadius = sin(fmod(cableV, 1.0f) / 0.3f * M_PI);
                        displacement += 0.015f * cableRadius;
                    }
                }
                
                // Add beam pipe panel lines (indented seams)
                if (vNormalized < 0.15f || vNormalized > 0.85f) {
                    if (segmentPos < 0.05f || segmentPos > 0.95f) {
                        displacement -= 0.02f; // Indented panel seams
                    }
                    
                    // Rivets/bolts (small bumps)
                    if (fmod(u * 128.0f, 1.0f) < 0.05f && 
                        (vNormalized < 0.05f || vNormalized > 0.95f)) {
                        float rivetSize = 0.015f;
                        displacement += rivetSize;
                    }
                }
                
                Vertex v_vertex;
                // Apply displacement along normal direction
                float effectiveRadius = MINOR_RADIUS + displacement;
                
                // Position with displacement
                v_vertex.x = (MAJOR_RADIUS + effectiveRadius * cosPhi) * cosTheta;
                v_vertex.y = (MAJOR_RADIUS + effectiveRadius * cosPhi) * sinTheta;
                v_vertex.z = effectiveRadius * sinPhi;
                
                // Normal (still pointing radially outward from tube)
                v_vertex.nx = cosPhi * cosTheta;
                v_vertex.ny = cosPhi * sinTheta;
                v_vertex.nz = sinPhi;
                
                // Texture coordinates
                v_vertex.u = u;
                v_vertex.v = v;
                
                vertices.push_back(v_vertex);
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

//- Additional 3D Structures Namespace
namespace Structures {
    
    // Draw a box (for equipment racks, vacuum pumps, etc.)
    void drawBox(float x, float y, float z, float w, float h, float d, 
                 float r, float g, float b) {
        glColor3f(r, g, b);
        
        // Front face
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(x-w/2, y-h/2, z+d/2);
        glVertex3f(x+w/2, y-h/2, z+d/2);
        glVertex3f(x+w/2, y+h/2, z+d/2);
        glVertex3f(x-w/2, y+h/2, z+d/2);
        glEnd();
        
        // Back face
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(x-w/2, y-h/2, z-d/2);
        glVertex3f(x-w/2, y+h/2, z-d/2);
        glVertex3f(x+w/2, y+h/2, z-d/2);
        glVertex3f(x+w/2, y-h/2, z-d/2);
        glEnd();
        
        // Top face
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(x-w/2, y+h/2, z-d/2);
        glVertex3f(x-w/2, y+h/2, z+d/2);
        glVertex3f(x+w/2, y+h/2, z+d/2);
        glVertex3f(x+w/2, y+h/2, z-d/2);
        glEnd();
        
        // Bottom face
        glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glVertex3f(x-w/2, y-h/2, z-d/2);
        glVertex3f(x+w/2, y-h/2, z-d/2);
        glVertex3f(x+w/2, y-h/2, z+d/2);
        glVertex3f(x-w/2, y-h/2, z+d/2);
        glEnd();
        
        // Right face
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(x+w/2, y-h/2, z-d/2);
        glVertex3f(x+w/2, y+h/2, z-d/2);
        glVertex3f(x+w/2, y+h/2, z+d/2);
        glVertex3f(x+w/2, y-h/2, z+d/2);
        glEnd();
        
        // Left face
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(x-w/2, y-h/2, z-d/2);
        glVertex3f(x-w/2, y-h/2, z+d/2);
        glVertex3f(x-w/2, y+h/2, z+d/2);
        glVertex3f(x-w/2, y+h/2, z-d/2);
        glEnd();
    }
    
    // Draw a cylinder (for pipes, supports)
    void drawCylinder(float x, float y, float z, float radius, float height, 
                      float r, float g, float b, bool horizontal = false) {
        const int slices = 12;
        glColor3f(r, g, b);
        
        for (int i = 0; i < slices; i++) {
            float angle1 = 2.0f * M_PI * i / slices;
            float angle2 = 2.0f * M_PI * (i + 1) / slices;
            
            float x1 = radius * cos(angle1);
            float y1 = radius * sin(angle1);
            float x2 = radius * cos(angle2);
            float y2 = radius * sin(angle2);
            
            glBegin(GL_QUADS);
            if (horizontal) {
                // Horizontal cylinder (along Y axis)
                glNormal3f(x1, 0, y1);
                glVertex3f(x + x1, y - height/2, z + y1);
                glVertex3f(x + x2, y - height/2, z + y2);
                glVertex3f(x + x2, y + height/2, z + y2);
                glVertex3f(x + x1, y + height/2, z + y1);
            } else {
                // Vertical cylinder (along Z axis)
                glNormal3f(x1, y1, 0);
                glVertex3f(x + x1, y + y1, z - height/2);
                glVertex3f(x + x2, y + y2, z - height/2);
                glVertex3f(x + x2, y + y2, z + height/2);
                glVertex3f(x + x1, y + y1, z + height/2);
            }
            glEnd();
        }
    }
    
    // Draw all structural elements around the accelerator
    void drawAll() {
        // Support pillars/stands every 45 degrees (8 total)
        for (int i = 0; i < 8; i++) {
            float angle = 2.0f * M_PI * i / 8.0f;
            float distance = Torus::MAJOR_RADIUS + 0.2f;
            float px = distance * cos(angle);
            float py = distance * sin(angle);
            float pz = -1.2f; // Below the torus
            
            // Support pillar (gray)
            drawBox(px, py, pz, 0.15f, 0.15f, 1.0f, 0.4f, 0.4f, 0.45f);
            
            // Connection bracket to torus (darker gray)
            drawBox(px * 0.85f, py * 0.85f, 0.1f, 0.12f, 0.12f, 0.3f, 0.3f, 0.3f, 0.35f);
        }
        
        // Vacuum pump stations (large boxes) every 90 degrees
        for (int i = 0; i < 4; i++) {
            float angle = 2.0f * M_PI * i / 4.0f + M_PI / 4.0f;
            float distance = Torus::MAJOR_RADIUS + 1.5f;
            float px = distance * cos(angle);
            float py = distance * sin(angle);
            
            // Main pump housing (industrial gray-green)
            drawBox(px, py, -0.3f, 0.4f, 0.4f, 0.6f, 0.35f, 0.4f, 0.35f);
            
            // Pump motor on top (darker)
            drawBox(px, py, 0.15f, 0.25f, 0.25f, 0.3f, 0.25f, 0.28f, 0.25f);
            
            // Connection pipe from pump to torus
            float torusX = (Torus::MAJOR_RADIUS + 0.5f) * cos(angle);
            float torusY = (Torus::MAJOR_RADIUS + 0.5f) * sin(angle);
            float midX = (px + torusX) / 2;
            float midY = (py + torusY) / 2;
            
            drawCylinder(midX, midY, -0.1f, 0.06f, 0.8f, 0.5f, 0.5f, 0.52f, true);
        }
        
        // Power supply units (every 60 degrees, 6 total)
        for (int i = 0; i < 6; i++) {
            float angle = 2.0f * M_PI * i / 6.0f;
            float distance = Torus::MAJOR_RADIUS + 1.0f;
            float px = distance * cos(angle);
            float py = distance * sin(angle);
            
            // Power supply box (beige/tan)
            drawBox(px, py, 0.5f, 0.3f, 0.3f, 0.4f, 0.55f, 0.52f, 0.45f);
            
            // Power indicator LEDs (green lights on front)
            float ledX = px + 0.15f * cos(angle + M_PI);
            float ledY = py + 0.15f * sin(angle + M_PI);
            drawBox(ledX, ledY, 0.65f, 0.03f, 0.03f, 0.02f, 0.2f, 0.9f, 0.2f);
        }
        
        // Cable conduits running between equipment
        for (int i = 0; i < 16; i++) {
            float angle = 2.0f * M_PI * i / 16.0f;
            float distance = Torus::MAJOR_RADIUS + 0.9f;
            float px = distance * cos(angle);
            float py = distance * sin(angle);
            
            // Cable conduit (dark gray)
            drawBox(px, py, -0.6f, 0.08f, 0.08f, 0.4f, 0.2f, 0.2f, 0.22f);
        }
        
        // Diagnostic sensor arrays (small boxes on top of torus)
        for (int i = 0; i < 24; i++) {
            float angle = 2.0f * M_PI * i / 24.0f;
            float distance = Torus::MAJOR_RADIUS;
            float px = distance * cos(angle);
            float py = distance * sin(angle);
            
            // Sensor box (white/light gray)
            drawBox(px, py, Torus::MINOR_RADIUS + 0.15f, 
                   0.12f, 0.12f, 0.08f, 0.8f, 0.82f, 0.85f);
        }
        
        // Cryogenic supply lines (blue pipes running alongside torus)
        const int numPipes = 4;
        for (int p = 0; p < numPipes; p++) {
            float offsetAngle = 2.0f * M_PI * p / numPipes;
            
            for (int i = 0; i < 32; i++) {
                float angle1 = 2.0f * M_PI * i / 32.0f;
                float angle2 = 2.0f * M_PI * (i + 1) / 32.0f;
                
                float r1 = Torus::MAJOR_RADIUS;
                float r2 = Torus::MAJOR_RADIUS;
                
                float pipeOffset = Torus::MINOR_RADIUS * 0.7f;
                
                float x1 = r1 * cos(angle1) + pipeOffset * cos(offsetAngle) * cos(angle1);
                float y1 = r1 * sin(angle1) + pipeOffset * cos(offsetAngle) * sin(angle1);
                float z1 = pipeOffset * sin(offsetAngle);
                
                float x2 = r2 * cos(angle2) + pipeOffset * cos(offsetAngle) * cos(angle2);
                float y2 = r2 * sin(angle2) + pipeOffset * cos(offsetAngle) * sin(angle2);
                float z2 = pipeOffset * sin(offsetAngle);
                
                // Small blue pipe segments
                glColor3f(0.2f, 0.4f, 0.7f);
                glBegin(GL_LINES);
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glEnd();
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
    
    // Rotate everything together
    glRotatef(rotateAngle, 0.3, 0.8, 0.1);
    
    // Draw main torus with surface details
    Torus::draw();
    
    // Draw additional 3D structures and equipment
    Structures::drawAll();
    
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
    
    cout << "Particle Accelerator Complex initialized!" << endl;
    cout << "Main Torus:" << endl;
    cout << "  Vertices: " << Torus::vertices.size() << endl;
    cout << "  Triangles: " << Torus::indices.size() / 3 << endl;
    cout << "  Surface features: Magnets, Detectors, Terminals, Coils, Cables" << endl;
    cout << "Additional 3D Structures:" << endl;
    cout << "  8 Support pillars with brackets" << endl;
    cout << "  4 Vacuum pump stations" << endl;
    cout << "  6 Power supply units" << endl;
    cout << "  16 Cable conduits" << endl;
    cout << "  24 Diagnostic sensor arrays" << endl;
    cout << "  Cryogenic supply lines" << endl;
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
