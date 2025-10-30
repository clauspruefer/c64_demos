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

## New: Donut Particle Accelerator (Enhanced)
**Location**: `test/opengl-donut-accelerator/opengl-textured-rotating-donut.cpp`

### Features:
- **Realistic torus (donut) geometry** - mathematically generated with thick tube
- **Advanced procedural texturing** - no external texture files needed
- **Filled polygon rendering** (`GL_FILL` mode) with smooth shading
- **Three-point lighting system** (key, fill, rim lights) for dramatic 3D depth
- **Modern particle accelerator appearance**:
  - Stainless steel beam pipe with panel lines and rivets
  - Dipole magnets (alternating red/blue, 32 sections)
  - Copper superconducting coils (visible windings)
  - Detector/monitoring sections with sensor grids
  - Energy beam glow (bright pulsing blue inner rim)
  - Cryogenic cooling pipes (blue horizontal lines)
  - Multi-colored cable trays (6 cable types)
  - Control terminals with LED indicators (green/red)
  - LCD display screens (cyan with text effect)
  - Panel edges and equipment housing
- Window size: 800×600 pixels
- Full depth testing and smooth shading
- Material properties with high shininess (80.0)
- Orbiting camera for optimal viewing

### Geometry:
```
Vertices: 4,704 (97 × 49)
Triangles: 9,216 (96 × 48 × 2)
Major Radius: 2.5
Minor Radius: 0.8
Segments: 96 major × 48 minor (smoother)
```

## Key Differences

| Feature | Cube | Donut (Enhanced) |
|---------|------|------------------|
| Shape | Cube (box) | Torus (thick donut) |
| Vertices | 8 | 4,704 |
| Triangles | 12 (6 faces) | 9,216 |
| Rendering | Wireframe | Filled with smooth shading |
| Texturing | Simple vertex colors | Advanced procedural (10+ patterns) |
| Lighting | None | Three-point lighting system |
| Complexity | Simple | Very high detail |
| Appearance | Geometric demo | Realistic particle accelerator |
| Components | None | Magnets, detectors, terminals, cables, cooling |
| Purpose | Precalc demo data | Visual demo/test |
| Realism | Low | High (LHC-inspired) |

## Technical Details

### Procedural Texture Patterns (Enhanced)

The donut uses UV coordinates and position to generate multiple realistic visual effects:

1. **Surface Region Detection**: Uses v-coordinate to determine position on torus tube:
   - Top (outer): Beam pipe surface
   - Inner (bottom): Energy beam channel
   - Upper sides: Magnet and detector equipment
   - Lower sides: Cables and control terminals

2. **Segmentation**: 32 sections around the ring with 4-segment pattern:
   - Segments 0,4,8,12,16,20,24,28: Red dipole magnets
   - Segments 1,5,9,13,17,21,25,29: Blue dipole magnets
   - Segments 2,6,10,14,18,22,26,30: Detector sections
   - Segments 3,7,11,15,19,23,27,31: Monitoring sections

3. **Material Textures**:
   - **Stainless steel**: (0.7, 0.72, 0.75) with panel lines
   - **Rivets/bolts**: Regular pattern at segment boundaries
   - **Magnet cores**: Alternating red (0.8, 0.2, 0.2) / blue (0.2, 0.3, 0.9)
   - **Copper coils**: (0.72, 0.45, 0.20) with winding pattern
   - **Detectors**: (0.85, 0.85, 0.88) with sensor grid
   - **Energy beam**: Pulsing blue (0.3-1.0 intensity) with sine wave
   - **Cooling pipes**: Horizontal blue lines (0.3, 0.6, 0.8)
   - **Cable tray**: Dark gray (0.25, 0.25, 0.28) base
   - **Individual cables**: 6 colors (red, blue, yellow, green, black, white)

4. **Terminal/Equipment Details**:
   - **Equipment racks**: Beige housing (0.65, 0.62, 0.58)
   - **LED indicators**: Green status (0.1, 0.9, 0.1), red warnings (0.9, 0.1, 0.1)
   - **LCD screens**: Cyan (0.1, 0.3, 0.4) with text lines (0.2, 0.8, 0.9)
   - **Panel borders**: Darkened edges for depth

5. **Weathering**: Random noise (±3%) for realistic surface variation

All of this simulates the appearance of a modern particle accelerator like the Large Hadron Collider (LHC) or RHIC, complete with superconducting magnets, cryogenic systems, detector arrays, and control equipment.

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
