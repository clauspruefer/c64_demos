//- OpenGL Three Spheres Display - GL_POINTS Vertex Model
//- Displays 3 overlapping spheres as a single point-based vertex model

#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Vertex structure with position and color
struct Vertex {
    float x, y, z;
    float r, g, b;
    double screenX, screenY, screenZ;  // Projected screen coordinates (GLdouble for gluProject)
};

// Sphere properties - positioned to overlap and form a single 3D model
namespace Spheres {
    // Sphere 1: Big, at top (moved down 20%)
    const float SPHERE1_RADIUS = 1.5f;
    const float SPHERE1_Y = 1.2f;  // Moved down 20% from 1.5 (extends from -0.3 to 2.7)
    
    // Sphere 2: Medium, in middle - overlaps with Sphere 1 (increased by 70%, moved down 30%)
    const float SPHERE2_RADIUS = 1.02f;
    const float SPHERE2_Y = -0.39f;  // Moved down 30% from -0.3 (extends from -1.41 to 0.63)
    
    // Sphere 3: Large, at bottom - overlaps with Sphere 2 (increased by 40%)
    const float SPHERE3_RADIUS = 1.4f;
    const float SPHERE3_Y = -1.5f;  // Overlaps with Sphere 2 (extends from -2.9 to -0.1)
    
    vector<Vertex> vertices;
    
    // Check if a point is inside any sphere
    bool isInsideSphere(float x, float y, float z, float sx, float sy, float sz, float radius) {
        float dx = x - sx;
        float dy = y - sy;
        float dz = z - sz;
        return (dx*dx + dy*dy + dz*dz) <= (radius*radius);
    }
    
    // Check if a point is occluded by another sphere (from camera viewpoint)
    bool isOccluded(float x, float y, float z, float depth) {
        // Check against all three spheres
        // A point is occluded if there's another sphere point closer to camera
        
        // Simple occlusion test: check if point is inside another sphere that's closer
        // Sphere 1 (closest in our view from the side)
        if (isInsideSphere(x, y, z, 0.0f, SPHERE1_Y, 0.0f, SPHERE1_RADIUS)) {
            // This point is on sphere 1's surface or inside
            float dist1 = sqrt(x*x + (y-SPHERE1_Y)*(y-SPHERE1_Y) + z*z);
            if (fabs(dist1 - SPHERE1_RADIUS) > 0.01f) {
                // Inside sphere 1, check if it's actually on another sphere's surface
                if (isInsideSphere(x, y, z, 0.0f, SPHERE2_Y, 0.0f, SPHERE2_RADIUS)) {
                    float dist2 = sqrt(x*x + (y-SPHERE2_Y)*(y-SPHERE2_Y) + z*z);
                    if (fabs(dist2 - SPHERE2_RADIUS) < 0.01f) return false; // On sphere 2 surface
                }
                if (isInsideSphere(x, y, z, 0.0f, SPHERE3_Y, 0.0f, SPHERE3_RADIUS)) {
                    float dist3 = sqrt(x*x + (y-SPHERE3_Y)*(y-SPHERE3_Y) + z*z);
                    if (fabs(dist3 - SPHERE3_RADIUS) < 0.01f) return false; // On sphere 3 surface
                }
                return true; // Inside sphere 1 but not on surface of others
            }
        }
        
        return false; // Not occluded
    }
    
    // Generate vertices for a sphere with pixel-level granularity
    void generateSphereVertices(float cx, float cy, float cz, float radius, 
                                float r, float g, float b, int sphereId) {
        // Calculate number of subdivisions to achieve ~1 pixel per vertex
        // Approximate based on radius and screen projection
        int segments = (int)(radius * 200); // Adjust multiplier for density
        if (segments < 50) segments = 50;
        if (segments > 500) segments = 500;
        
        // Generate points using spherical coordinates
        for (int i = 0; i <= segments; i++) {
            float theta = M_PI * i / segments; // 0 to PI
            
            for (int j = 0; j <= segments * 2; j++) {
                float phi = 2.0f * M_PI * j / (segments * 2); // 0 to 2PI
                
                // Calculate sphere surface point
                float x = cx + radius * sin(theta) * cos(phi);
                float y = cy + radius * cos(theta);
                float z = cz + radius * sin(theta) * sin(phi);
                
                // Check if this vertex is occluded (skip internal overlapping vertices)
                if (!isOccluded(x, y, z, z)) {
                    Vertex v;
                    v.x = x;
                    v.y = y;
                    v.z = z;
                    v.r = r;
                    v.g = g;
                    v.b = b;
                    vertices.push_back(v);
                }
            }
        }
    }
    
    void generateAllVertices() {
        vertices.clear();
        
        cout << "Generating point-based vertex model..." << endl;
        
        // Generate vertices for all three spheres
        // Order matters - generate from back to front for proper depth
        generateSphereVertices(0.0f, SPHERE3_Y, 0.0f, SPHERE3_RADIUS, 0.2f, 0.2f, 0.9f, 3);
        generateSphereVertices(0.0f, SPHERE2_Y, 0.0f, SPHERE2_RADIUS, 0.2f, 0.9f, 0.2f, 2);
        generateSphereVertices(0.0f, SPHERE1_Y, 0.0f, SPHERE1_RADIUS, 0.9f, 0.2f, 0.2f, 1);
        
        cout << "Generated " << vertices.size() << " vertices" << endl;
    }
    
    void projectVertices() {
        // Get current matrices for projection
        GLdouble modelview[16];
        GLdouble projection[16];
        GLint viewport[4];
        
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        // Project all vertices to screen coordinates
        for (size_t i = 0; i < vertices.size(); i++) {
            gluProject(vertices[i].x, vertices[i].y, vertices[i].z,
                      modelview, projection, viewport,
                      &vertices[i].screenX, &vertices[i].screenY, &vertices[i].screenZ);
        }
    }
    
    void drawAll() {
        // Project vertices for this frame
        projectVertices();
        
        // Draw all vertices as points
        glPointSize(2.0f);  // Adjust point size as needed
        glBegin(GL_POINTS);
        for (size_t i = 0; i < vertices.size(); i++) {
            glColor3f(vertices[i].r, vertices[i].g, vertices[i].b);
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        }
        glEnd();
    }
    
    void exportVertices(const char* filename) {
        // Export vertices to file for external use
        FILE* f = fopen(filename, "w");
        if (!f) return;
        
        fprintf(f, "# Three Spheres Vertex Model\n");
        fprintf(f, "# Format: x y z r g b screenX screenY screenZ\n");
        fprintf(f, "# Total vertices: %zu\n\n", vertices.size());
        
        for (size_t i = 0; i < vertices.size(); i++) {
            fprintf(f, "%f %f %f %f %f %f %f %f %f\n",
                   vertices[i].x, vertices[i].y, vertices[i].z,
                   vertices[i].r, vertices[i].g, vertices[i].b,
                   vertices[i].screenX, vertices[i].screenY, vertices[i].screenZ);
        }
        
        fclose(f);
        cout << "Exported vertices to " << filename << endl;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Position camera to view the combined 3D model
    gluLookAt(
        5.0, 0.0, 8.0,   // Eye position - slightly to the right
        0.0, 0.0, 0.0,   // Look at center of the model
        0.0, 1.0, 0.0    // Up vector
    );
    
    // Draw all vertices as points
    Spheres::drawAll();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'e' || key == 'E') {
        // Export vertices when 'e' is pressed
        Spheres::exportVertices("vertices.txt");
    } else if (key == 27) {  // ESC key
        exit(0);
    }
}

void init() {
    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Disable lighting for point rendering (we control color directly)
    glDisable(GL_LIGHTING);
    
    // Enable point smoothing for better appearance
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    // Set background color
    glClearColor(0.1, 0.1, 0.15, 1.0);
    
    // Generate the vertex model
    Spheres::generateAllVertices();
    
    cout << "\nThree Spheres Point-Based Vertex Model initialized!" << endl;
    cout << "Sphere 1: Radius = " << Spheres::SPHERE1_RADIUS << ", Y position = " << Spheres::SPHERE1_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE1_Y - Spheres::SPHERE1_RADIUS) << " to " << (Spheres::SPHERE1_Y + Spheres::SPHERE1_RADIUS) << endl;
    cout << "Sphere 2: Radius = " << Spheres::SPHERE2_RADIUS << ", Y position = " << Spheres::SPHERE2_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE2_Y - Spheres::SPHERE2_RADIUS) << " to " << (Spheres::SPHERE2_Y + Spheres::SPHERE2_RADIUS) << endl;
    cout << "Sphere 3: Radius = " << Spheres::SPHERE3_RADIUS << ", Y position = " << Spheres::SPHERE3_Y << endl;
    cout << "  - Y range: " << (Spheres::SPHERE3_Y - Spheres::SPHERE3_RADIUS) << " to " << (Spheres::SPHERE3_Y + Spheres::SPHERE3_RADIUS) << endl;
    cout << "\nPress 'E' to export vertices to file" << endl;
    cout << "Press 'ESC' to exit" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Three Spheres - GL_POINTS Vertex Model");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}
