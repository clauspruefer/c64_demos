//- OpenGL Morphing 3D Models - 128 Vertex Point Cloud Animation
//- Five different 3D models that morph into each other every 60 seconds

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>

using namespace std;

static const int FPS = 60;
static const int NUM_VERTICES = 128;
static const int SECONDS_PER_MODEL = 60;
static const int FRAMES_PER_MODEL = SECONDS_PER_MODEL * FPS;
static const int MORPH_DURATION = 2 * FPS; // 2 seconds morph transition

static int frameCounter = 0;
static GLint rotateAngle = 0;

//- Model namespace containing all 5 3D models
namespace Models {
    
    // Model structure
    struct Vertex {
        float x, y, z;
        float r, g, b;  // Color
    };
    
    vector<Vertex> model1; // Sphere
    vector<Vertex> model2; // Cube
    vector<Vertex> model3; // Torus (donut)
    vector<Vertex> model4; // Double Helix
    vector<Vertex> model5; // Icosahedron-like star
    
    vector<Vertex> currentModel;
    
    // Generate sphere model with 128 vertices
    void generateSphere() {
        model1.clear();
        const int rings = 8;
        const int sectors = 16;
        const float radius = 2.5f;
        
        for (int i = 0; i < rings; i++) {
            float theta = M_PI * (float)i / (float)(rings - 1);
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            
            for (int j = 0; j < sectors; j++) {
                float phi = 2.0f * M_PI * (float)j / (float)sectors;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);
                
                Vertex v;
                v.x = radius * sinTheta * cosPhi;
                v.y = radius * sinTheta * sinPhi;
                v.z = radius * cosTheta;
                
                // Blue gradient
                v.r = 0.2f + 0.3f * (float)i / rings;
                v.g = 0.4f + 0.4f * (float)i / rings;
                v.b = 0.8f + 0.2f * (float)j / sectors;
                
                model1.push_back(v);
            }
        }
        
        cout << "Sphere vertices: " << model1.size() << endl;
    }
    
    // Generate cube model with 128 vertices
    void generateCube() {
        model2.clear();
        const float size = 3.0f;
        const int pointsPerEdge = 8;
        const int pointsPerFace = 16;  // Number of points per cube face
        
        // Generate points on cube edges and faces
        for (int i = 0; i < pointsPerEdge; i++) {
            float t = -size/2 + size * (float)i / (float)(pointsPerEdge - 1);
            
            for (int j = 0; j < pointsPerEdge; j++) {
                float s = -size/2 + size * (float)j / (float)(pointsPerEdge - 1);
                
                // Face 1: z = size/2
                if ((i + j * pointsPerEdge) < pointsPerFace) {
                    Vertex v;
                    v.x = t;
                    v.y = s;
                    v.z = size/2;
                    v.r = 1.0f;
                    v.g = 0.3f + 0.3f * (float)i / pointsPerEdge;
                    v.b = 0.3f + 0.3f * (float)j / pointsPerEdge;
                    model2.push_back(v);
                }
                
                // Face 2: z = -size/2
                if ((i + j * pointsPerEdge) < pointsPerFace) {
                    Vertex v;
                    v.x = t;
                    v.y = s;
                    v.z = -size/2;
                    v.r = 0.3f + 0.3f * (float)i / pointsPerEdge;
                    v.g = 1.0f;
                    v.b = 0.3f + 0.3f * (float)j / pointsPerEdge;
                    model2.push_back(v);
                }
            }
        }
        
        // Add edge vertices
        for (int i = 0; i < pointsPerEdge; i++) {
            float t = -size/2 + size * (float)i / (float)(pointsPerEdge - 1);
            
            // Edges along x-axis
            for (int k = 0; k < 4; k++) {
                Vertex v;
                v.x = t;
                v.y = (k % 2 == 0) ? -size/2 : size/2;
                v.z = (k < 2) ? -size/2 : size/2;
                v.r = 0.8f;
                v.g = 0.8f;
                v.b = 0.3f;
                model2.push_back(v);
                if (model2.size() >= NUM_VERTICES) break;
            }
            if (model2.size() >= NUM_VERTICES) break;
        }
        
        // Pad to 128 if needed
        if (!model2.empty()) {
            while (model2.size() < NUM_VERTICES) {
                model2.push_back(model2.back());
            }
        } else {
            // Fallback: create a simple cube if generation failed
            for (int i = 0; i < NUM_VERTICES; i++) {
                Vertex v;
                v.x = ((i % 4) - 1.5f) * 1.5f;
                v.y = (((i / 4) % 4) - 1.5f) * 1.5f;
                v.z = (((i / 16) % 4) - 1.5f) * 1.5f;
                v.r = 0.8f;
                v.g = 0.8f;
                v.b = 0.3f;
                model2.push_back(v);
            }
        }
        
        // Trim to exactly 128
        model2.resize(NUM_VERTICES);
        
        cout << "Cube vertices: " << model2.size() << endl;
    }
    
    // Generate torus (donut) model with 128 vertices
    void generateTorus() {
        model3.clear();
        const float majorRadius = 2.0f;
        const float minorRadius = 0.8f;
        const int majorSegments = 16;
        const int minorSegments = 8;
        
        for (int i = 0; i < majorSegments; i++) {
            float theta = 2.0f * M_PI * (float)i / (float)majorSegments;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            
            for (int j = 0; j < minorSegments; j++) {
                float phi = 2.0f * M_PI * (float)j / (float)minorSegments;
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);
                
                Vertex v;
                v.x = (majorRadius + minorRadius * cosPhi) * cosTheta;
                v.y = (majorRadius + minorRadius * cosPhi) * sinTheta;
                v.z = minorRadius * sinPhi;
                
                // Pink/purple gradient
                v.r = 0.8f + 0.2f * (float)i / majorSegments;
                v.g = 0.3f + 0.3f * (float)j / minorSegments;
                v.b = 0.8f;
                
                model3.push_back(v);
            }
        }
        
        cout << "Torus vertices: " << model3.size() << endl;
    }
    
    // Generate double helix model with 128 vertices
    void generateDoubleHelix() {
        model4.clear();
        const float radius = 1.5f;
        const float height = 6.0f;
        const int pointsPerHelix = 64;
        
        // First helix
        for (int i = 0; i < pointsPerHelix; i++) {
            float t = (float)i / (float)(pointsPerHelix - 1);
            float angle = t * 4.0f * M_PI;
            
            Vertex v;
            v.x = radius * cos(angle);
            v.y = radius * sin(angle);
            v.z = -height/2 + height * t;
            
            // Cyan gradient
            v.r = 0.2f;
            v.g = 0.8f + 0.2f * t;
            v.b = 0.9f;
            
            model4.push_back(v);
        }
        
        // Second helix (offset by 180 degrees)
        for (int i = 0; i < pointsPerHelix; i++) {
            float t = (float)i / (float)(pointsPerHelix - 1);
            float angle = t * 4.0f * M_PI + M_PI;
            
            Vertex v;
            v.x = radius * cos(angle);
            v.y = radius * sin(angle);
            v.z = -height/2 + height * t;
            
            // Yellow gradient
            v.r = 0.9f;
            v.g = 0.9f - 0.2f * t;
            v.b = 0.2f;
            
            model4.push_back(v);
        }
        
        cout << "Double Helix vertices: " << model4.size() << endl;
    }
    
    // Generate icosahedron-like star model with 128 vertices
    void generateStar() {
        model5.clear();
        const float innerRadius = 1.0f;
        const float outerRadius = 3.5f;
        const int numSpikes = 16;
        const int pointsPerSpike = 8;
        
        for (int i = 0; i < numSpikes; i++) {
            // Horizontal angle
            float theta = 2.0f * M_PI * (float)i / (float)numSpikes;
            
            // Vertical angle variation
            float phi = M_PI / 4.0f * sin(2.0f * M_PI * (float)i / (float)numSpikes);
            
            for (int j = 0; j < pointsPerSpike; j++) {
                float t = (float)j / (float)(pointsPerSpike - 1);
                float r = innerRadius + (outerRadius - innerRadius) * t;
                
                Vertex v;
                v.x = r * cos(theta) * cos(phi);
                v.y = r * sin(theta) * cos(phi);
                v.z = r * sin(phi);
                
                // Rainbow gradient
                float hue = (float)i / numSpikes;
                v.r = 0.5f + 0.5f * cos(hue * 2.0f * M_PI);
                v.g = 0.5f + 0.5f * cos((hue + 0.33f) * 2.0f * M_PI);
                v.b = 0.5f + 0.5f * cos((hue + 0.67f) * 2.0f * M_PI);
                
                model5.push_back(v);
            }
        }
        
        cout << "Star vertices: " << model5.size() << endl;
    }
    
    // Initialize all models
    void generateAll() {
        generateSphere();
        generateCube();
        generateTorus();
        generateDoubleHelix();
        generateStar();
        
        // Start with first model
        currentModel = model1;
    }
    
    // Linear interpolation between two vertices
    Vertex lerp(const Vertex& a, const Vertex& b, float t) {
        Vertex result;
        result.x = a.x + (b.x - a.x) * t;
        result.y = a.y + (b.y - a.y) * t;
        result.z = a.z + (b.z - a.z) * t;
        result.r = a.r + (b.r - a.r) * t;
        result.g = a.g + (b.g - a.g) * t;
        result.b = a.b + (b.b - a.b) * t;
        return result;
    }
    
    // Update current model based on time (morphing logic)
    void update(int frame) {
        int totalFrames = 5 * FRAMES_PER_MODEL;
        frame = frame % totalFrames;
        
        int modelIndex = frame / FRAMES_PER_MODEL;
        int frameInModel = frame % FRAMES_PER_MODEL;
        
        // Determine which models to interpolate between
        const vector<Vertex>* fromModel;
        const vector<Vertex>* toModel;
        float morphProgress = 0.0f;
        
        // Select models based on current time
        switch (modelIndex) {
            case 0: fromModel = &model1; toModel = &model2; break;
            case 1: fromModel = &model2; toModel = &model3; break;
            case 2: fromModel = &model3; toModel = &model4; break;
            case 3: fromModel = &model4; toModel = &model5; break;
            case 4: fromModel = &model5; toModel = &model1; break;
            default: fromModel = &model1; toModel = &model1; break;
        }
        
        // Calculate morph progress during last 2 seconds of display
        if (frameInModel >= FRAMES_PER_MODEL - MORPH_DURATION) {
            int morphFrame = frameInModel - (FRAMES_PER_MODEL - MORPH_DURATION);
            morphProgress = (float)morphFrame / (float)MORPH_DURATION;
            // Smooth interpolation using smoothstep
            morphProgress = morphProgress * morphProgress * (3.0f - 2.0f * morphProgress);
        }
        
        // Interpolate vertices
        currentModel.clear();
        for (int i = 0; i < NUM_VERTICES; i++) {
            Vertex v = lerp((*fromModel)[i], (*toModel)[i], morphProgress);
            currentModel.push_back(v);
        }
        
        // Debug output every 60 frames (once per second)
        if (frame % 60 == 0) {
            cout << "Frame: " << frame 
                 << " | Model: " << (modelIndex + 1) 
                 << " | Morph: " << (int)(morphProgress * 100) << "%" << endl;
        }
    }
    
    // Draw current model using GL_POINTS
    void draw() {
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        for (size_t i = 0; i < currentModel.size(); i++) {
            const Vertex& v = currentModel[i];
            glColor3f(v.r, v.g, v.b);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    // Rotate the model
    glRotatef(rotateAngle, 0.5, 0.8, 0.3);
    
    // Update and draw current model
    Models::update(frameCounter);
    Models::draw();
    
    rotateAngle += 1;
    frameCounter++;
    
    glPopMatrix();
    
    glutSwapBuffers();
}

void timer(int v) {
    static GLfloat u = 0.0;
    u += 0.01;
    
    glLoadIdentity();
    
    // Camera orbits around the models
    float camDist = 10.0f;
    gluLookAt(
        camDist * cos(u), 
        camDist * sin(u * 0.5f) * 0.5f, 
        camDist * sin(u) * 0.5f + 5.0f,
        0.0, 0.0, 0.0,
        0, 1, 0
    );
    
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, v);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Set background color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Enable point smoothing for nicer looking points
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    // Generate all models
    Models::generateAll();
    
    cout << "\n=== 3D Morphing Models Demo ===" << endl;
    cout << "Total vertices per model: " << NUM_VERTICES << endl;
    cout << "Display time per model: " << SECONDS_PER_MODEL << " seconds" << endl;
    cout << "Morph transition time: " << (MORPH_DURATION / FPS) << " seconds" << endl;
    cout << "Total animation cycle: " << (5 * SECONDS_PER_MODEL) << " seconds" << endl;
    cout << "\nModel sequence:" << endl;
    cout << "  1. Sphere (blue)" << endl;
    cout << "  2. Cube (red/green)" << endl;
    cout << "  3. Torus/Donut (pink/purple)" << endl;
    cout << "  4. Double Helix (cyan/yellow)" << endl;
    cout << "  5. Star (rainbow)" << endl;
    cout << "================================\n" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Morphing Models - GL_POINTS Demo");
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
