//- OpenGL Textured Cylinder
//- Displays a cylinder with 150px diameter in 320x200 window
//- Texture: 16 pixels width (x) and 300 pixels height (y) mapped from bottom to top

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

static const int FPS = 60;
static GLfloat rotateAngle = 0.0f;

//- Cylinder Namespace
namespace Cylinder {
    const float RADIUS = 75.0f;      // 150px diameter = 75px radius
    const float HEIGHT = 300.0f;     // Height matches texture height
    const int SEGMENTS = 32;         // Segments around the cylinder
    const int HEIGHT_SEGMENTS = 32;  // Vertical segments for smooth mapping
    
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;  // Normal
        float u, v;        // Texture coordinates
    };
    
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    
    // Texture parameters
    const int TEX_WIDTH = 16;
    const int TEX_HEIGHT = 300;
    GLuint textureID;
    
    // Generate procedural texture
    void generateTexture() {
        unsigned char* texData = new unsigned char[TEX_WIDTH * TEX_HEIGHT * 3];
        
        for (int y = 0; y < TEX_HEIGHT; y++) {
            for (int x = 0; x < TEX_WIDTH; x++) {
                int idx = (y * TEX_WIDTH + x) * 3;
                
                // Create a gradient pattern with vertical stripes
                float normalizedY = (float)y / TEX_HEIGHT;
                
                // Vertical gradient (bottom to top: blue to cyan to white)
                float r, g, b;
                if (normalizedY < 0.33f) {
                    // Bottom third: dark blue to blue
                    r = 0.1f + normalizedY * 0.3f;
                    g = 0.2f + normalizedY * 0.6f;
                    b = 0.5f + normalizedY * 1.5f;
                } else if (normalizedY < 0.66f) {
                    // Middle third: blue to cyan
                    float t = (normalizedY - 0.33f) / 0.33f;
                    r = 0.2f + t * 0.3f;
                    g = 0.4f + t * 0.6f;
                    b = 1.0f;
                } else {
                    // Top third: cyan to white
                    float t = (normalizedY - 0.66f) / 0.34f;
                    r = 0.5f + t * 0.5f;
                    g = 1.0f;
                    b = 1.0f;
                }
                
                // Add vertical stripes
                if (fmod(x, 4) < 2) {
                    r *= 0.85f;
                    g *= 0.85f;
                    b *= 0.85f;
                }
                
                // Add horizontal bands every 50 pixels
                if (fmod(y, 50) < 3) {
                    r = 1.0f;
                    g = 1.0f;
                    b = 1.0f;
                }
                
                texData[idx + 0] = (unsigned char)(r * 255);
                texData[idx + 1] = (unsigned char)(g * 255);
                texData[idx + 2] = (unsigned char)(b * 255);
            }
        }
        
        // Generate and bind texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, texData);
        
        delete[] texData;
    }
    
    // Generate cylinder geometry
    void generate() {
        vertices.clear();
        indices.clear();
        
        // Generate vertices
        for (int i = 0; i <= HEIGHT_SEGMENTS; i++) {
            float v = (float)i / HEIGHT_SEGMENTS;
            float y = HEIGHT * (v - 0.5f); // Center vertically
            
            for (int j = 0; j <= SEGMENTS; j++) {
                float u = (float)j / SEGMENTS;
                float theta = 2.0f * M_PI * u;
                
                Vertex vertex;
                vertex.x = RADIUS * cos(theta);
                vertex.z = RADIUS * sin(theta);
                vertex.y = y;
                
                // Normal points outward
                vertex.nx = cos(theta);
                vertex.ny = 0.0f;
                vertex.nz = sin(theta);
                
                // Texture coordinates
                vertex.u = u;
                vertex.v = v;
                
                vertices.push_back(vertex);
            }
        }
        
        // Generate indices
        for (int i = 0; i < HEIGHT_SEGMENTS; i++) {
            for (int j = 0; j < SEGMENTS; j++) {
                int first = i * (SEGMENTS + 1) + j;
                int second = first + SEGMENTS + 1;
                
                // First triangle
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);
                
                // Second triangle
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
        
        // Add top cap
        int topCenterIdx = vertices.size();
        Vertex topCenter;
        topCenter.x = 0.0f;
        topCenter.y = HEIGHT * 0.5f;
        topCenter.z = 0.0f;
        topCenter.nx = 0.0f;
        topCenter.ny = 1.0f;
        topCenter.nz = 0.0f;
        topCenter.u = 0.5f;
        topCenter.v = 1.0f;
        vertices.push_back(topCenter);
        
        for (int j = 0; j < SEGMENTS; j++) {
            int idx = HEIGHT_SEGMENTS * (SEGMENTS + 1) + j;
            indices.push_back(topCenterIdx);
            indices.push_back(idx);
            indices.push_back(idx + 1);
        }
        
        // Add bottom cap
        int bottomCenterIdx = vertices.size();
        Vertex bottomCenter;
        bottomCenter.x = 0.0f;
        bottomCenter.y = -HEIGHT * 0.5f;
        bottomCenter.z = 0.0f;
        bottomCenter.nx = 0.0f;
        bottomCenter.ny = -1.0f;
        bottomCenter.nz = 0.0f;
        bottomCenter.u = 0.5f;
        bottomCenter.v = 0.0f;
        vertices.push_back(bottomCenter);
        
        for (int j = 0; j < SEGMENTS; j++) {
            int idx = j;
            indices.push_back(bottomCenterIdx);
            indices.push_back(idx + 1);
            indices.push_back(idx);
        }
    }
    
    // Draw the cylinder
    void draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < indices.size(); i++) {
            unsigned int idx = indices[i];
            const Vertex& v = vertices[idx];
            
            glTexCoord2f(v.u, v.v);
            glNormal3f(v.nx, v.ny, v.nz);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    // Rotate the cylinder
    glRotatef(rotateAngle, 0.0, 1.0, 0.0);
    
    // Draw cylinder
    Cylinder::draw();
    
    rotateAngle += 1.0f;
    if (rotateAngle >= 360.0f) rotateAngle -= 360.0f;
    
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
    
    // Set up orthographic projection to match pixel coordinates
    float aspect = (float)w / (float)h;
    float halfHeight = 160.0f;
    float halfWidth = halfHeight * aspect;
    
    glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -500.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Position camera to view the cylinder
    gluLookAt(0.0, 0.0, 400.0,   // Eye position
              0.0, 0.0, 0.0,      // Look at center
              0.0, 1.0, 0.0);     // Up vector
}

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light
    GLfloat light_position[] = { 200.0, 200.0, 300.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
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
    
    // Set background color
    glClearColor(0.1, 0.1, 0.15, 1.0);
    
    // Generate texture and geometry
    Cylinder::generateTexture();
    Cylinder::generate();
    
    cout << "Textured Cylinder initialized!" << endl;
    cout << "Window size: 320x200 pixels" << endl;
    cout << "Cylinder diameter: 150 pixels (radius: 75 pixels)" << endl;
    cout << "Texture size: " << Cylinder::TEX_WIDTH << "x" << Cylinder::TEX_HEIGHT << " pixels" << endl;
    cout << "Vertices: " << Cylinder::vertices.size() << endl;
    cout << "Triangles: " << Cylinder::indices.size() / 3 << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(320, 200);
    glutCreateWindow("OpenGL Textured Cylinder - 320x200");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
