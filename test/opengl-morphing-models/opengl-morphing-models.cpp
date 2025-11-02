//- OpenGL Morphing 3D Models - 64 Vertex Point Cloud Animation
//- Two different 3D models that morph into each other

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

static const int FPS = 10;  // 10 frames per second
static const int NUM_VERTICES = 64;
static const int FRAMES_PER_LOOP = 180;  // Each animation loops in 180 frames
static const int LOOPS_BEFORE_MORPH = 3;  // Loop 3 times before morphing
static const int FRAMES_PER_MODEL = FRAMES_PER_LOOP * LOOPS_BEFORE_MORPH;  // 540 frames
static const int MORPH_DURATION = 180;  // 180 frames morph transition
static const int EXPORT_INTERVAL = 1;  // Export every frame for PNG sequence

static int frameCounter = 0;
static int globalFrameCounter = 0;  // For PNG export numbering

//- Model namespace containing 2 3D models
namespace Models {
    
    // Model structure
    struct Vertex {
        float x, y, z;
        float r, g, b;  // Color
    };
    
    vector<Vertex> model1; // Torus (donut)
    vector<Vertex> model2; // Star
    
    vector<Vertex> currentModel;
    
    // Generate torus (donut) model with 64 vertices
    void generateTorus() {
        model1.clear();
        const float majorRadius = 2.0f;
        const float minorRadius = 0.8f;
        const int majorSegments = 16;
        const int minorSegments = 4;
        
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
                
                model1.push_back(v);
            }
        }
        
        cout << "Torus vertices: " << model1.size() << endl;
    }
    
    // Generate icosahedron-like star model with 64 vertices
    void generateStar() {
        model2.clear();
        const float innerRadius = 1.0f;
        const float outerRadius = 3.5f;
        const int numSpikes = 16;
        const int pointsPerSpike = 4;
        
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
                
                model2.push_back(v);
            }
        }
        
        cout << "Star vertices: " << model2.size() << endl;
    }
    
    // Initialize all models
    void generateAll() {
        generateTorus();
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
        int totalFrames = 2 * FRAMES_PER_MODEL;  // 2 models total
        frame = frame % totalFrames;
        
        int modelIndex = frame / FRAMES_PER_MODEL;
        int frameInModel = frame % FRAMES_PER_MODEL;
        
        // Determine which models to interpolate between
        const vector<Vertex>* fromModel;
        const vector<Vertex>* toModel;
        float morphProgress = 0.0f;
        
        // Select models based on current time (only 2 models now)
        switch (modelIndex) {
            case 0: fromModel = &model1; toModel = &model2; break;
            case 1: fromModel = &model2; toModel = &model1; break;
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
            int loopNum = (frameInModel / FRAMES_PER_LOOP) + 1;
            int frameInLoop = frameInModel % FRAMES_PER_LOOP;
            cout << "Frame: " << frame 
                 << " | Model: " << (modelIndex + 1) 
                 << " | Loop: " << loopNum << "/2"
                 << " | Frame in loop: " << frameInLoop
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

// Save current frame as PNG
void saveFrameAsPNG() {
    int width = 800;
    int height = 600;
    
    // Allocate memory for pixel data
    unsigned char* pixels = new unsigned char[width * height * 3];
    
    // Read pixels from framebuffer
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    // Create filename
    stringstream ss;
    ss << "frame_" << setfill('0') << setw(6) << globalFrameCounter << ".ppm";
    string filename = ss.str();
    
    // Save as PPM (simple format that can be converted to PNG)
    ofstream file(filename, ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    
    // Flip vertically (OpenGL's origin is bottom-left)
    for (int y = height - 1; y >= 0; y--) {
        file.write((char*)(pixels + y * width * 3), width * 3);
    }
    
    file.close();
    delete[] pixels;
    
    cout << "Saved " << filename << " (frame " << globalFrameCounter << ")" << endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    // Calculate rotation angle based on frame number within loop
    // This makes the rotation independent from morphing
    int totalFrames = 2 * FRAMES_PER_MODEL;  // 2 models total
    int currentFrame = frameCounter % totalFrames;
    int frameInModel = currentFrame % FRAMES_PER_MODEL;
    
    // Determine if we're in morph phase or loop phase
    bool inMorphPhase = frameInModel >= FRAMES_PER_MODEL - MORPH_DURATION;
    
    float rotationAngle;
    if (inMorphPhase) {
        // During morph, continue rotation from last loop
        int morphFrame = frameInModel - (FRAMES_PER_MODEL - MORPH_DURATION);
        rotationAngle = 360.0f + (360.0f * morphFrame / (float)MORPH_DURATION);
    } else {
        // During loop phase, rotate based on frame within loop
        int loopFrame = frameInModel % FRAMES_PER_LOOP;
        rotationAngle = 360.0f * loopFrame / (float)FRAMES_PER_LOOP;
    }
    
    // Multi-axis rotation - all axes complete full rotations in 180 frames for seamless looping
    // Y-axis: 1 full rotation (360°) in 180 frames
    glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    // X-axis: 2 full rotations (720°) in 180 frames for more dynamic movement
    glRotatef(rotationAngle * 2.0f, 1.0, 0.0, 0.0);
    // Z-axis: 1 full rotation (360°) in 180 frames
    glRotatef(rotationAngle, 0.0, 0.0, 1.0);
    
    // Update and draw current model
    Models::update(frameCounter);
    Models::draw();
    
    // Export coordinates every frame using gluProject
    if (frameCounter % EXPORT_INTERVAL == 0) {
        // Get the current matrices and viewport
        GLdouble modelview[16];
        GLdouble projection[16];
        GLint viewport[4];
        
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        GLdouble projectedCoords[NUM_VERTICES][3];
        
        // Project all vertices to window coordinates
        for (int i = 0; i < NUM_VERTICES; i++) {
            gluProject(
                (GLdouble)Models::currentModel[i].x,
                (GLdouble)Models::currentModel[i].y,
                (GLdouble)Models::currentModel[i].z,
                modelview,
                projection,
                viewport,
                &projectedCoords[i][0],
                &projectedCoords[i][1],
                &projectedCoords[i][2]
            );
            cout << "Vertex " << i << " -> Window coords: (" 
                 << projectedCoords[i][0] << ", " 
                 << projectedCoords[i][1] << ")" << endl;
        }
    }
    
    glPopMatrix();
    
    glutSwapBuffers();
    
    // Save frame as PNG for video export
    saveFrameAsPNG();
    
    frameCounter++;
    globalFrameCounter++;
}

void timer(int v) {
    glLoadIdentity();
    
    // Fixed camera position (no orbiting/zooming)
    float camDist = 10.0f;
    gluLookAt(
        0.0, 0.0, camDist,  // Camera position - fixed on Z axis
        0.0, 0.0, 0.0,      // Look at origin
        0, 1, 0             // Up vector
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
    cout << "FPS: " << FPS << endl;
    cout << "Total vertices per model: " << NUM_VERTICES << endl;
    cout << "Frames per loop: " << FRAMES_PER_LOOP << " (" << (FRAMES_PER_LOOP / FPS) << " seconds)" << endl;
    cout << "Loops before morph: " << LOOPS_BEFORE_MORPH << endl;
    cout << "Display time per model: " << (FRAMES_PER_MODEL / FPS) << " seconds" << endl;
    cout << "Morph transition duration: " << MORPH_DURATION << " frames (" << (MORPH_DURATION / FPS) << " seconds)" << endl;
    cout << "Total animation cycle: " << (2 * (FRAMES_PER_MODEL + MORPH_DURATION) / FPS) << " seconds" << endl;
    cout << "Total frames for complete loop: " << (2 * (FRAMES_PER_MODEL + MORPH_DURATION)) << " frames" << endl;
    cout << "\nPNG Export: Saving every frame as PPM (convert to PNG with ImageMagick)" << endl;
    cout << "Camera: Fixed position (no zooming/orbiting)" << endl;
    cout << "\nModel sequence:" << endl;
    cout << "  1. Torus/Donut (pink/purple) - loops 3 times" << endl;
    cout << "  2. Torus -> Star morph (180 frames)" << endl;
    cout << "  3. Star (rainbow) - loops 3 times" << endl;
    cout << "  4. Star -> Torus morph (180 frames)" << endl;
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
