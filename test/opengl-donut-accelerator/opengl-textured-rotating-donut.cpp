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
    // Torus parameters
    const float MAJOR_RADIUS = 2.0f;  // Distance from center to tube center
    const float MINOR_RADIUS = 0.5f;  // Tube radius
    const int MAJOR_SEGMENTS = 64;     // Segments around the major circle
    const int MINOR_SEGMENTS = 32;     // Segments around the tube
    
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
    
    // Draw the torus with procedural texturing
    void draw() {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < indices.size(); i++) {
            unsigned int idx = indices[i];
            const Vertex& v = vertices[idx];
            
            // Use texture coordinates to generate procedural color
            // Simulate particle accelerator appearance
            float u = v.u;
            float vCoord = v.v;
            
            // Create bands effect for accelerator segments
            float bandPattern = floor(u * 16.0f);
            float bandColor = (fmod(bandPattern, 2.0f) < 1.0f) ? 0.3f : 0.5f;
            
            // Add wire pattern
            float wirePattern = 0.0f;
            if (fmod(vCoord * 32.0f, 1.0f) < 0.1f || fmod(u * 32.0f, 1.0f) < 0.1f) {
                wirePattern = 0.3f;
            }
            
            // Terminal indicators (bright spots)
            float terminalPattern = 0.0f;
            if (fmod(u * 8.0f, 1.0f) < 0.15f && fmod(vCoord, 0.5f) < 0.2f) {
                terminalPattern = 0.5f;
            }
            
            // Metal/steel base color with blue glow
            float r = 0.5f + bandColor + wirePattern;
            float g = 0.5f + bandColor + wirePattern;
            float b = 0.7f + bandColor + wirePattern + terminalPattern;
            
            // Add energy glow effect on inner rim
            if (vCoord > 0.4f && vCoord < 0.6f) {
                r += 0.2f;
                g += 0.4f;
                b += 0.6f;
            }
            
            // Add terminal screens (greenish displays)
            if (fmod(u * 8.0f, 1.0f) > 0.85f && vCoord > 0.3f && vCoord < 0.7f) {
                r = 0.2f;
                g = 0.8f;
                b = 0.3f;
            }
            
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
    u += 0.02;
    glLoadIdentity();
    
    // Position camera to view the donut from various angles
    gluLookAt(
        8*cos(u), 7*cos(u)-1, 4*cos(u/3)+2,  // Eye position
        0.0, 0.0, 0.0,                        // Look at center
        0, 1, 0                                // Up vector
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
    glEnable(GL_LIGHT0);
    
    // Set up light
    GLfloat light_position[] = { 5.0, 5.0, 5.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.4, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 1.0, 1.0 };
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
    
    // Set background color (dark space-like)
    glClearColor(0.05, 0.05, 0.1, 1.0);
    
    // Generate torus geometry
    Torus::generate();
    
    cout << "Particle Accelerator Donut initialized!" << endl;
    cout << "Vertices: " << Torus::vertices.size() << endl;
    cout << "Triangles: " << Torus::indices.size() / 3 << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle Accelerator Ring - Donut Demo");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
