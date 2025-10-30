//- Enhanced OpenGL cylinder with texture mapped on the helix points
//- Based on the original double helix demo, adds texture between the two helix strands
//- Texture displays "this is a cylinder opengl demo by twf/hitmen" vertically

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <cstring>

#include <glm/vec3.hpp>

using namespace std;

static const int FPS = 60;
static GLfloat rotateAngle = 0.0f;
static GLuint textureID = 0;

//- Double Helix Namespace
namespace DoubleHelix {

    const int NUM_VERTICES = 18;

    //- Double helix vertices: two intertwined ropes wrapping around cylinder
    //- These are arranged in pairs: strand1[i] at even indices, strand2[i] at odd indices
    GLint vertices[NUM_VERTICES][3] = {
      {  8,  15,   0}, { -8,  15,   0}, 
      { -7,  11,  -5}, {  7,  11,   5}, 
      {  6,   8,   4}, { -6,   8,  -4}, 
      { -5,   4,  -8}, {  5,   4,   8}, 
      { -3,   0,   7}, {  3,   0,  -7}, 
      {  2,  -4,  -9}, { -2,  -4,   9}, 
      { -4,  -8,   6}, {  4,  -8,  -6}, 
      {  8, -11,  -1}, { -8, -11,   1}, 
      { -8, -15,   0}, {  8, -15,   0}
    };

    GLfloat vertexColors[NUM_VERTICES][3] = {
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, 
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, 
      {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}
    };

    void drawPoints() {
      // Draw double helix vertices as points (original visualization)
      glBegin(GL_POINTS);
      for (int i = 0; i < NUM_VERTICES; i++) {
        glColor3fv((GLfloat*)&vertexColors[i]);
        glVertex3iv((GLint*)&vertices[i]);
      }
      glEnd();
    }

    void drawTexturedSurface() {
      // Draw textured surface connecting each strand to itself (creating a ribbon)
      // We'll create two separate ribbons, one for each strand
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textureID);
      glColor3f(1.0f, 1.0f, 1.0f);  // White color to show texture properly
      
      // Create a single ribbon connecting both strands
      // Use GL_QUAD_STRIP where texture wraps around following the helix
      glBegin(GL_QUAD_STRIP);
      
      // Iterate through all vertices sequentially to follow the helix spiral
      for (int i = 0; i < NUM_VERTICES; i++) {
        // Calculate texture coordinates
        // s: wraps around as we follow the spiral (0.0 to 1.0 and repeat)
        // t: goes from bottom to top (0.0 to 1.0)
        float t = (float)i / (NUM_VERTICES - 1);
        float s = (i % 2 == 0) ? 0.0f : 1.0f;  // Alternate between edges
        
        glTexCoord2f(s, t);
        glVertex3iv((GLint*)&vertices[i]);
      }
      
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
}

//- Create texture with vertical text
void createTexture() {
    const int TEX_WIDTH = 256;
    const int TEX_HEIGHT = 512;
    const char* text = "this is a cylinder opengl demo by twf/hitmen";
    
    // Create texture data (RGBA format)
    unsigned char* textureData = new unsigned char[TEX_WIDTH * TEX_HEIGHT * 4];
    
    // Initialize to dark blue background
    for (int i = 0; i < TEX_WIDTH * TEX_HEIGHT * 4; i += 4) {
        textureData[i] = 0x20;      // R
        textureData[i + 1] = 0x20;  // G
        textureData[i + 2] = 0x80;  // B (blue)
        textureData[i + 3] = 0xFF;  // A
    }
    
    // Simple character rendering (bitmap font simulation)
    // Each character is 8x8 pixels
    const int CHAR_WIDTH = 8;
    const int CHAR_HEIGHT = 8;
    const int CHAR_SPACING = 2;
    
    int textLen = strlen(text);
    int startY = (TEX_HEIGHT - (textLen * (CHAR_HEIGHT + CHAR_SPACING))) / 2;
    
    // Draw each character vertically (from bottom to top)
    for (int charIdx = 0; charIdx < textLen; charIdx++) {
        char c = text[charIdx];
        int baseY = startY + charIdx * (CHAR_HEIGHT + CHAR_SPACING);
        int baseX = (TEX_WIDTH - CHAR_WIDTH) / 2;
        
        // Simple bitmap for each character (5x7 within 8x8 grid)
        // This is a very simplified font - just rendering rectangles for demo
        unsigned char pattern[8][8] = {0};
        
        // Create simple patterns for letters (very basic)
        if (c >= 'a' && c <= 'z') {
            // Draw a simple box pattern for each letter
            for (int i = 1; i < 7; i++) {
                for (int j = 1; j < 7; j++) {
                    if (i == 1 || i == 6 || j == 1 || j == 6) {
                        pattern[i][j] = 1;
                    }
                }
            }
            // Add distinguishing features based on character
            int charOffset = c - 'a';
            for (int i = 2; i < 6; i++) {
                pattern[i][2 + (charOffset % 3)] = 1;
            }
        } else if (c == ' ') {
            // Space - leave empty
        } else if (c == '/') {
            // Diagonal line
            for (int i = 0; i < 8; i++) {
                pattern[i][7 - i] = 1;
            }
        } else {
            // Other characters - simple vertical line
            for (int i = 1; i < 7; i++) {
                pattern[i][4] = 1;
            }
        }
        
        // Draw the character into texture
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (pattern[y][x]) {
                    int pixelY = baseY + y;
                    int pixelX = baseX + x;
                    if (pixelX >= 0 && pixelX < TEX_WIDTH && pixelY >= 0 && pixelY < TEX_HEIGHT) {
                        int idx = (pixelY * TEX_WIDTH + pixelX) * 4;
                        textureData[idx] = 0xFF;      // R (white)
                        textureData[idx + 1] = 0xFF;  // G
                        textureData[idx + 2] = 0xFF;  // B
                        textureData[idx + 3] = 0xFF;  // A
                    }
                }
            }
        }
    }
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    
    delete[] textureData;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(rotateAngle, 0.0, 1.0, 0.0);

    // Draw textured surface connecting the helix strands
    DoubleHelix::drawTexturedSurface();
    
    // Draw helix points on top for visibility
    DoubleHelix::drawPoints();
    
    // Increment by 11.25 degrees per frame for 32-frame loop
    rotateAngle += 11.25f;
    if (rotateAngle >= 360.0f) rotateAngle -= 360.0f;

    glFlush();
    glutSwapBuffers();
}

void timer(int v) {
  glLoadIdentity();
  gluLookAt(0.0, 35.0, 55.0,     // Eye position
            0.0, 0.0, 0.0,       // Look at center
            0.0, 4.0, 0.0);      // Up vector
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, GLfloat(w) / GLfloat(h), 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);  // Enable depth testing for proper rendering
  
  // Create the texture with vertical text
  createTexture();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(420, 2520);
  glutCreateWindow("Textured Double Helix - TWF/Hitmen");
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
