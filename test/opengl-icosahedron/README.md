# OpenGL Animated Icosahedron

A visually stunning OpenGL demonstration featuring a rotating icosahedron (20-faced polyhedron) with realistic shading and dual light sources.

## Features

- **Icosahedron Geometry**: Perfect 20-faced polyhedron with 12 vertices
- **Realistic Shading**: Smooth shading with filled triangular faces
- **Dual Light Sources**: Two lights positioned at left front and right front for dynamic illumination
- **Complete Animation**: 180 frames for full rotation on X, Y, and Z axes
- **Material Properties**: Specular highlights and ambient/diffuse lighting

## Technical Details

### Geometry
- 12 vertices based on the golden ratio (φ = (1+√5)/2)
- 20 triangular faces forming a regular icosahedron
- Proper face normals calculated for each triangle

### Lighting
- **Light 0** (Left Front): Position (-5, 3, 5)
  - Main light source with stronger diffuse component
- **Light 1** (Right Front): Position (5, 3, 5)
  - Secondary fill light with moderate intensity

### Animation
- Total frames: 180
- Frame rate: 60 FPS
- Duration: 3 seconds per complete rotation
- Simultaneous rotation on all three axes (X, Y, Z)
- Each axis completes exactly 360° rotation in 180 frames

## Building and Running

### Prerequisites
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)
- g++ compiler with C++11 support

### On Ubuntu/Debian
```bash
sudo apt-get install freeglut3-dev
```

### Build
```bash
make
```

### Run
```bash
make run
```

Or directly:
```bash
./opengl-icosahedron
```

### Clean
```bash
make clean
```

## Mathematical Background

An icosahedron is one of the five Platonic solids - a convex polyhedron with:
- 20 equilateral triangular faces
- 12 vertices
- 30 edges

The vertices are positioned at coordinates involving the golden ratio, creating a perfectly symmetric 3D shape.

## Visual Effect

The icosahedron appears with:
- Blue-tinted material with specular highlights
- Smooth shading across faces
- Dynamic lighting from two sources creating realistic depth
- Continuous smooth rotation showing all faces over time
