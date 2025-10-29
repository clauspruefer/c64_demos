# Comparison: Cube vs. Donut Particle Accelerator

## Original: Colored Rotating Cube
**Location**: `demos/cubism/part2/opengl/opengl-colored-rotating-cube.cpp`

### Features:
- Simple 8-vertex cube geometry
- 6 colored faces
- Wireframe rendering (`GL_LINE` polygon mode)
- Basic rotation
- Vertex color interpolation
- Pixel visibility detection
- Window size: 96×80 pixels

### Geometry:
```
Vertices: 8
Faces: 6 (quads)
```

## New: Donut Particle Accelerator
**Location**: `test/opengl-donut-accelerator/opengl-textured-rotating-donut.cpp`

### Features:
- **Torus (donut) geometry** - mathematically generated
- **Procedural texturing** - no external texture files needed
- **Filled polygon rendering** (`GL_FILL` mode)
- **Full 3D lighting** with specular highlights
- **Particle accelerator appearance**:
  - Segmented bands (16 sections around the ring)
  - Wire patterns (grid overlay)
  - Terminal indicators with bright spots (8 terminals)
  - Energy glow effect on inner rim (blue glow)
  - Green terminal screens positioned at intervals
  - Metallic steel base color
- Window size: 800×600 pixels
- Depth testing enabled
- Material properties with shininess

### Geometry:
```
Vertices: 2,145 (65 × 33)
Triangles: 4,096 (64 × 32 × 2)
Major Radius: 2.0
Minor Radius: 0.5
```

## Key Differences

| Feature | Cube | Donut |
|---------|------|-------|
| Shape | Cube (box) | Torus (donut) |
| Vertices | 8 | 2,145 |
| Rendering | Wireframe | Filled with lighting |
| Texturing | Simple vertex colors | Procedural texturing |
| Complexity | Simple | High detail |
| Appearance | Geometric demo | Particle accelerator ring |
| Lighting | None | Full 3D lighting |
| Purpose | Precalc demo data | Visual demo/test |

## Technical Details

### Procedural Texture Patterns

The donut uses UV coordinates to generate multiple visual effects:

1. **Band Pattern**: `floor(u * 16.0)` creates 16 segments around the ring
2. **Wire Pattern**: Grid lines at regular intervals using `fmod()`
3. **Terminal Pattern**: Bright spots at 8 locations around the ring
4. **Energy Glow**: Bright blue glow on the inner rim (v: 0.4-0.6)
5. **Terminal Screens**: Green displays at specific UV positions

All of this simulates the appearance of a giant particle accelerator like the Large Hadron Collider (LHC), complete with technical equipment and wiring.

## Building

Both programs require OpenGL/GLUT libraries:
```bash
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglm-dev
```

The donut program includes a Makefile:
```bash
cd test/opengl-donut-accelerator
make
./donut-accelerator
```
