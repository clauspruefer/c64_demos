# OpenGL Donut Particle Accelerator

This is a 3D OpenGL demonstration that renders a giant donut (torus) designed to look like a modern particle accelerator ring (similar to the Large Hadron Collider) with **real 3D geometric variations** on the surface and **multiple additional 3D structures** surrounding the main ring.

## Features

### Main Torus with 3D Surface Geometry
- **Realistic Torus Geometry**: Mathematically generated donut with actual geometric surface variations (not just textures!)
- **Physical Surface Details** (X,Y,Z displacement):
  - Equipment terminal boxes protruding 0.15 units from surface
  - Magnet coil windings (physical ridges with sine wave pattern)
  - Cooling pipe ridges (cylindrical cross-sections)
  - Detector housing boxes
  - Cable tray walls with individual cable bumps
  - Beam pipe panel seams (indented grooves)
  - Rivets/bolts (small raised bumps)

### Additional 3D Structures (58+ Objects)
- **8 Support Pillars**: Gray vertical supports below torus with connection brackets
- **4 Vacuum Pump Stations**: Large equipment boxes with motors and connection pipes
- **6 Power Supply Units**: Beige boxes with green LED indicators
- **16 Cable Conduits**: Dark gray boxes running between equipment
- **24 Diagnostic Sensor Arrays**: White sensor boxes mounted on top of torus
- **Cryogenic Supply Lines**: Blue pipes running alongside the torus

### Advanced Procedural Texturing
No external texture files needed - all visual effects are generated procedurally:
- Stainless steel beam pipe with panel lines
- Alternating red/blue dipole magnets (32 sections)
- Copper superconducting coils with visible windings
- Detector/monitoring sections with sensor grids
- Pulsing energy beam glow (bright blue inner rim)
- Cryogenic cooling pipes (blue horizontal lines)
- Multi-colored cable trays (red, blue, yellow, green, black, white cables)
- Control terminals with LED indicators (green status, red warnings)
- LCD display screens with scrolling text effect
- Panel edges and equipment housing

### Professional Rendering
- **Three-Point Lighting System** (key, fill, rim lights) for dramatic 3D depth
- **Smooth Geometry**: 96×48 segment mesh (4,704 vertices) for main torus
- **Dynamic Camera**: Orbiting camera to show all angles of the 3D structure
- **Material Properties**: High shininess (80.0) for metallic surfaces
- **Depth Testing**: Proper 3D occlusion

## Visual Details

### Segment Layout
The accelerator ring is divided into 32 segments around the major circle:
- Segments 0, 4, 8, 12, 16, 20, 24, 28: Red dipole magnets with copper coils
- Segments 1, 5, 9, 13, 17, 21, 25, 29: Blue dipole magnets with copper coils
- Segments 2, 6, 10, 14, 18, 22, 26, 30: Detector/monitoring sections
- Segments 3, 7, 11, 15, 19, 23, 27, 31: Detector/monitoring sections
- Every 8th segment: Control terminal with LEDs and LCD display

### Surface Regions
- **Top (outer)**: Beam pipe (stainless steel, ~15% of circumference)
- **Inner (bottom)**: Energy beam (bright pulsing blue, ~10% of circumference)
- **Upper sides**: Magnets and detectors (~30% of circumference)
- **Lower sides**: Cable trays and control terminals (~45% of circumference)

## Building

### Requirements
- g++ compiler with C++11 support
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)
- GLM (OpenGL Mathematics library)

### On Ubuntu/Debian:
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

### Compile:
```bash
make
```

### Run:
```bash
make run
```

Or directly:
```bash
./donut-accelerator
```

## Based On

This demo is based on `demos/cubism/part2/opengl/opengl-colored-rotating-cube.cpp` but completely replaces the cube geometry with a torus and adds extensive procedural texturing to simulate a realistic particle accelerator ring.

## Technical Details

- **Main Torus**:
  - Major Radius: 2.5 (distance from center to tube center)
  - Minor Radius: 0.8 + displacement (tube radius with 3D surface variations)
  - Segments: 96 major segments × 48 minor segments
  - Vertices: 4,704 (97 × 49)
  - Triangles: 9,216 (96 × 48 × 2)
  - Surface Displacement Range: -0.02 to +0.15 units
- **Additional Structures**: 58+ separate 3D objects
- **Window Size**: 800×600 pixels
- **Frame Rate**: 60 FPS
- **Rendering**: Triple-buffered with depth testing, smooth shading, and three-point lighting
- **Lights**: Key light (top-right), fill light (left), rim light (back)

## 3D Geometry Features

### Surface Displacement Map
The torus surface has real geometric variations (not just texture):

```
Terminal boxes:      +0.15 units (protrudes outward)
Coil ridges:         ±0.02 units (sine wave pattern)
Cooling pipes:       +0.04 units (cylindrical bumps)
Detector housings:   +0.08 units (rectangular protrusions)
Cable bumps:         +0.015 units (individual cables)
Panel seams:         -0.02 units (indented grooves)
Rivets:              +0.015 units (small bumps)
```

### Additional 3D Objects
All objects are actual 3D geometry with proper normals and lighting:

- **Support Pillars**: 8 boxes (0.15×0.15×1.0) with brackets (0.12×0.12×0.3)
- **Vacuum Pumps**: 4 main housings (0.4×0.4×0.6) + motors (0.25×0.25×0.3) + connection pipes
- **Power Supplies**: 6 boxes (0.3×0.3×0.4) + LED indicators (0.03×0.03×0.02)
- **Cable Conduits**: 16 boxes (0.08×0.08×0.4)
- **Sensor Arrays**: 24 boxes (0.12×0.12×0.08)
- **Cryo Lines**: 4 pipes wrapping around torus (128 segments each)

## Improvements Over Original Version

- Increased segment count (64→96 major, 32→48 minor) for smoother appearance
- Thicker torus (0.5→0.8 minor radius) for better donut look
- Much more realistic particle accelerator appearance with actual components
- Three-point lighting instead of single light
- Better camera movement for showing 3D depth
- Detailed procedural textures instead of simple bands
- Proper equipment: magnets, detectors, terminals, cables, cooling systems
