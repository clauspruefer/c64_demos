// OpenGL Eyes to Cubes Animation
// Creates 2 realistic moving eyes with pupils that morph into rotating colored cubes

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>

using namespace std;

static const int FPS = 60;
static float animationTime = 0.0f;
static const float MORPH_START = 3.0f;  // Start morphing after 3 seconds
static const float MORPH_DURATION = 4.0f;  // Morph over 4 seconds
static float cubeRotation = 0.0f;

// Eye parameters
static float eyeLookAngleH = 0.0f;  // Horizontal look angle
static float eyeLookAngleV = 0.0f;  // Vertical look angle

// Pupil colors (will be used for cube textures)
static const float PUPIL_COLOR_LEFT[3] = {0.2f, 0.4f, 0.8f};   // Blue
static const float PUPIL_COLOR_RIGHT[3] = {0.6f, 0.3f, 0.7f};  // Purple

// Calculate morph factor (0.0 = eye, 1.0 = cube)
float getMorphFactor() {
    if (animationTime < MORPH_START) {
        return 0.0f;
    }
    float t = (animationTime - MORPH_START) / MORPH_DURATION;
    if (t > 1.0f) t = 1.0f;
    // Smooth ease-in-out
    return t * t * (3.0f - 2.0f * t);
}

// Draw a sphere with given parameters
void drawSphere(float radius, int slices, int stacks) {
    glutSolidSphere(radius, slices, stacks);
}

// Draw an eye (sclera, iris, and pupil)
void drawEye(const float pupilColor[3]) {
    float morphFactor = getMorphFactor();
    
    // Sclera (white part of eye)
    glColor3f(0.95f, 0.95f, 0.95f);
    drawSphere(1.0f, 32, 32);
    
    if (morphFactor < 0.99f) {
        // Iris (colored part around pupil)
        glPushMatrix();
        glTranslatef(
            sin(eyeLookAngleH) * 0.3f,
            sin(eyeLookAngleV) * 0.3f,
            cos(eyeLookAngleH) * cos(eyeLookAngleV) * 0.95f
        );
        
        // Iris color (lighter version of pupil color)
        glColor3f(
            pupilColor[0] * 0.7f + 0.3f,
            pupilColor[1] * 0.7f + 0.3f,
            pupilColor[2] * 0.7f + 0.3f
        );
        drawSphere(0.4f * (1.0f - morphFactor * 0.5f), 24, 24);
        
        // Pupil (dark center)
        glColor3fv(pupilColor);
        drawSphere(0.2f * (1.0f - morphFactor * 0.8f), 16, 16);
        glPopMatrix();
    }
}

// Draw a cube with colored faces
void drawColoredCube(const float color[3]) {
    float size = 0.8f;
    
    glBegin(GL_QUADS);
    
    // Front face
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    
    // Back face
    glColor3f(color[0] * 0.7f, color[1] * 0.7f, color[2] * 0.7f);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);
    
    // Top face
    glColor3f(color[0] * 0.9f, color[1] * 0.9f, color[2] * 0.9f);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);
    
    // Bottom face
    glColor3f(color[0] * 0.6f, color[1] * 0.6f, color[2] * 0.6f);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    
    // Right face
    glColor3f(color[0] * 0.8f, color[1] * 0.8f, color[2] * 0.8f);
    glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glVertex3f(size, -size, size);
    
    // Left face
    glColor3f(color[0] * 0.75f, color[1] * 0.75f, color[2] * 0.75f);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);
    
    glEnd();
}

// Draw a morphing eye/cube
void drawMorphingObject(const float pupilColor[3], float offsetX) {
    float morphFactor = getMorphFactor();
    
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, 0.0f);
    
    if (morphFactor < 1.0f) {
        // Rotate during morphing for transition effect
        glRotatef(morphFactor * 180.0f, 0.0f, 1.0f, 0.0f);
    } else {
        // Fast rotation for fully morphed cubes
        glRotatef(cubeRotation, 1.0f, 1.0f, 0.0f);
    }
    
    if (morphFactor < 0.99f) {
        // Draw eye with decreasing opacity during morph
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Scale down eye during morphing
        glPushMatrix();
        float eyeScale = 1.0f - morphFactor * 0.3f;
        glScalef(eyeScale, eyeScale, eyeScale);
        drawEye(pupilColor);
        glPopMatrix();
        
        glDisable(GL_BLEND);
    }
    
    if (morphFactor > 0.01f) {
        // Draw cube with increasing opacity during morph
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Scale up cube during morphing
        float cubeScale = morphFactor;
        glPushMatrix();
        glScalef(cubeScale, cubeScale, cubeScale);
        drawColoredCube(pupilColor);
        glPopMatrix();
        
        glDisable(GL_BLEND);
    }
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera position
    gluLookAt(0.0, 0.0, 6.0,   // Eye position
              0.0, 0.0, 0.0,    // Look at center
              0.0, 1.0, 0.0);   // Up vector
    
    // Enable lighting for realistic look
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light
    GLfloat lightPos[] = {2.0f, 3.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // Material properties
    GLfloat matSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat matShininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    
    // Draw left morphing object
    drawMorphingObject(PUPIL_COLOR_LEFT, -2.0f);
    
    // Draw right morphing object
    drawMorphingObject(PUPIL_COLOR_RIGHT, 2.0f);
    
    glDisable(GL_LIGHTING);
    
    glutSwapBuffers();
}

void timer(int v) {
    // Update animation time
    animationTime += 1.0f / FPS;
    
    // Animate eye looking around (before morphing)
    if (animationTime < MORPH_START) {
        eyeLookAngleH = sin(animationTime * 1.5f) * 0.4f;
        eyeLookAngleV = cos(animationTime * 2.0f) * 0.3f;
    }
    
    // Rotate cubes fast after morphing is complete
    if (getMorphFactor() >= 0.99f) {
        cubeRotation += 5.0f;  // Fast rotation
        if (cubeRotation >= 360.0f) {
            cubeRotation -= 360.0f;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, v);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // Dark blue background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Eyes to Cubes Animation");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(1000 / FPS, timer, 0);
    init();
    
    cout << "Animation sequence:" << endl;
    cout << "0-3s: Two realistic eyes looking around" << endl;
    cout << "3-7s: Eyes morph into colored rotating cubes" << endl;
    cout << "7s+: Fast rotating cubes with pupil colors" << endl;
    
    glutMainLoop();
    return 0;
}
