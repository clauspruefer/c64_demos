# Three Spheres OpenGL 3D Display - GL_POINTS Vertex Model

This OpenGL C++ program displays **3 overlapping spheres as a single GL_POINTS-based vertex model**. The spheres form a cohesive 3D model where each vertex represents approximately a single pixel in 3D-2D transformation, and all vertices are exportable via gluProject() with their z-coordinates.

## Sphere Specifications

### Sphere 1 (Red) - Top
- **Radius**: `1.5`
- **Y Position**: `1.2` (moved down 20% from 1.5)
- **Y Range**: `-0.3 to 2.7`

### Sphere 2 (Green) - Middle (Overlapping)
- **Radius**: `1.02` (increased by 70% from 0.6)
- **Y Position**: `-0.39` (moved down 30% from -0.3)
- **Y Range**: `-1.41 to 0.63`
- **Note**: Overlaps with Sphere 1 (both occupy Y range -0.3 to 0.63)

### Sphere 3 (Blue) - Bottom (Overlapping)
- **Radius**: `1.4` (increased by 40% from 1.0)
- **Y Position**: `-1.5`
- **Y Range**: `-2.9 to -0.1`
- **Note**: Overlaps with Sphere 2 (both occupy Y range -1.41 to -0.1)

## Key Features

- **GL_POINTS rendering**: Single vertex model using OpenGL points instead of solid spheres
- **Pixel-level granularity**: Each vertex represents approximately one pixel on screen
- **Occlusion culling**: Overlapped/non-visible vertices are omitted from the model
- **gluProject() compatible**: All vertices can be projected to screen coordinates with z-depth
- **Vertex export**: Press 'E' to export all vertices with 3D and projected screen coordinates
- **Unified model**: Three spheres combined into a single point-based representation

## Vertex Model Details

- **Total vertices**: ~400,000 points (varies based on sphere sizes)
- **Vertex density**: Automatically calculated for pixel-level accuracy
- **Occlusion handling**: Internal overlapping vertices are excluded
- **Data export format**: `x y z r g b screenX screenY screenZ`

## Build and Run

```bash
cd tools/three-spheres
make
./three-spheres
```

### Keyboard Controls

- **E**: Export vertices to `vertices.txt` file
- **ESC**: Exit program

### Vertex Export

When you press 'E', the program exports all vertices to `vertices.txt` with the following format:
```
x y z r g b screenX screenY screenZ
```

Each line contains:
- `x y z`: 3D world coordinates
- `r g b`: Color values (0.0 to 1.0)
- `screenX screenY screenZ`: Projected 2D screen coordinates and depth

## Dependencies

- OpenGL
- GLUT (freeglut3-dev)
- GLU (libglu1-mesa-dev)

On Ubuntu/Debian:
```bash
sudo apt-get install freeglut3-dev libglu1-mesa-dev mesa-common-dev
```

## Technical Details

- ✓ OpenGL GL_POINTS rendering for vertex-based model
- ✓ Three overlapping spheres combined into single point cloud
- ✓ Pixel-level vertex density for accurate screen representation
- ✓ Depth testing enabled for proper 3D occlusion
- ✓ Automatic occlusion culling of overlapped vertices
- ✓ gluProject() integration for 3D-to-2D coordinate mapping
- ✓ Each vertex exportable with full 3D and screen coordinates
- ✓ Point smoothing enabled for better visual appearance

## Files

- `three-spheres.cpp` - Main C++ source code
- `Makefile` - Build configuration
- `README.md` - This file
