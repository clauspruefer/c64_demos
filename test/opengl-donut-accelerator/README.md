# OpenGL Donut Particle Accelerator

This is a 3D OpenGL demonstration that renders a giant donut (torus) designed to look like a modern particle accelerator ring (similar to the Large Hadron Collider) with realistic technical details including computer terminals, magnets, detectors, beam pipes, cooling systems, and cable trays.

## Features

- **Realistic Torus Geometry**: A mathematically generated donut shape with increased thickness for better visual appearance
- **Advanced Procedural Texturing**: No external texture files needed - all visual effects are generated procedurally
- **Particle Accelerator Components**:
  - **Stainless steel beam pipe**: Outer surface with panel lines and rivets
  - **Dipole magnets**: Alternating red/blue magnet sections (representing north/south poles)
  - **Copper superconducting coils**: Visible coil windings on magnet sections
  - **Detector/monitoring sections**: White sensor grid patterns for particle detection
  - **Energy beam glow**: Bright blue pulsing glow on inner rim (simulating particle beam)
  - **Cryogenic cooling pipes**: Blue horizontal lines representing liquid helium cooling
  - **Cable trays**: Complex multi-colored power and signal cables (red, blue, yellow, green, black, white)
  - **Control terminals**: Equipment racks with LED indicators (green status, red warnings)
  - **LCD display screens**: Cyan displays with scrolling text effect
  - **Panel edges and borders**: Realistic equipment housing
- **Professional Lighting**: Three-point lighting system (key, fill, rim) for dramatic 3D depth
- **Smooth Geometry**: 96×48 segment mesh (4,704 vertices) for smooth appearance
- **Dynamic Camera**: Orbiting camera to show all angles of the 3D structure

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

- **Major Radius**: 2.5 (distance from center to tube center)
- **Minor Radius**: 0.8 (tube radius - thicker for better donut appearance)
- **Segments**: 96 major segments × 48 minor segments
- **Vertices**: 4,704 (97 × 49)
- **Triangles**: 9,216 (96 × 48 × 2)
- **Window Size**: 800×600 pixels
- **Frame Rate**: 60 FPS
- **Rendering**: Triple-buffered with depth testing, smooth shading, and three-point lighting
- **Lights**: Key light (top-right), fill light (left), rim light (back)

## Improvements Over Original Version

- Increased segment count (64→96 major, 32→48 minor) for smoother appearance
- Thicker torus (0.5→0.8 minor radius) for better donut look
- Much more realistic particle accelerator appearance with actual components
- Three-point lighting instead of single light
- Better camera movement for showing 3D depth
- Detailed procedural textures instead of simple bands
- Proper equipment: magnets, detectors, terminals, cables, cooling systems
