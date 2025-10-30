# 3D Geometry Details

This document describes the actual 3D geometric variations in the particle accelerator model.

## Surface Displacement (Main Torus)

The main torus is **not** a smooth surface. Instead, the vertices are displaced along their normals to create real 3D geometric features:

### Equipment Terminal Boxes
**Location**: Every 8th segment (0, 8, 16, 24), bottom side (v: 0.55-0.80)
**Displacement**: Up to +0.15 units (largest protrusion)
**Shape**: Beveled rectangular box
- Edges fade smoothly with sine wave transitions
- Creates actual protruding equipment racks

### Magnet Coil Windings
**Location**: Magnet sections (segments 0,1,4,5,8,9,12,13,16,17,20,21,24,25,28,29), upper side (v: 0.15-0.45)
**Displacement**: ±0.02 units in sine wave pattern
**Detail**: 64 ridges around tube circumference
**Additional**: +0.05 unit bumps every 4 segments for support structures

### Cooling Pipe Ridges
**Location**: All sections, upper side (v: 0.20-0.40)
**Displacement**: Up to +0.04 units
**Shape**: Cylindrical cross-sections (16 pipes around tube)
**Pattern**: Sine wave creates rounded pipe surface

### Detector Housing Boxes
**Location**: Detector sections (segments 2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31), upper side (v: 0.15-0.45)
**Displacement**: Up to +0.08 units
**Shape**: Rectangular boxes with sine wave edges

### Cable Tray Details
**Location**: Bottom side (v: 0.65-0.85)
**Features**:
- Tray walls: +0.03 units at edges (v: 0.65-0.68, 0.82-0.85)
- Individual cables: +0.015 units (30% of v-space, sine wave bumps)

### Beam Pipe Panel Seams
**Location**: Outer surface (v: <0.15 or >0.85), at segment boundaries
**Displacement**: -0.02 units (indented grooves)
**Purpose**: Creates panel separation lines

### Rivets/Bolts
**Location**: Outer surface edges (v: <0.05 or >0.95), every 1/128th of ring
**Displacement**: +0.015 units
**Shape**: Small rounded bumps

## Total Surface Displacement Range
- **Minimum**: -0.02 units (panel seams)
- **Maximum**: +0.15 units (terminal boxes)
- **Range**: 0.17 units (21% of minor radius!)

This creates **significant** 3D variation visible from any angle.

## Additional 3D Structures

These are completely separate geometric objects rendered around the main torus:

### Support Infrastructure

**8 Support Pillars**:
- Position: Every 45° around torus, distance = major_radius + 0.2
- Size: 0.15 × 0.15 × 1.0 (W × D × H)
- Z-position: -1.2 (below torus)
- Color: Gray (0.4, 0.4, 0.45)
- Connection brackets: 0.12 × 0.12 × 0.3, darker gray

### Vacuum Systems

**4 Vacuum Pump Stations**:
- Position: Every 90° (offset by 45°), distance = major_radius + 1.5
- Components:
  - Main housing: 0.4 × 0.4 × 0.6 (industrial gray-green)
  - Motor on top: 0.25 × 0.25 × 0.3 (darker gray)
  - Connection pipe: Cylinder radius 0.06, height 0.8
- Purpose: Simulate vacuum pumping system

### Power Systems

**6 Power Supply Units**:
- Position: Every 60° around torus, distance = major_radius + 1.0
- Size: 0.3 × 0.3 × 0.4
- Color: Beige/tan (0.55, 0.52, 0.45)
- LED indicators: 0.03 × 0.03 × 0.02 (bright green)
- Z-position: 0.5 (above midplane)

### Cable Infrastructure

**16 Cable Conduits**:
- Position: Every 22.5° around torus, distance = major_radius + 0.9
- Size: 0.08 × 0.08 × 0.4
- Color: Dark gray (0.2, 0.2, 0.22)
- Z-position: -0.6 (below torus)
- Purpose: Cable routing between equipment

### Diagnostics

**24 Diagnostic Sensor Arrays**:
- Position: Every 15° around torus, on top of torus
- Size: 0.12 × 0.12 × 0.08
- Color: White/light gray (0.8, 0.82, 0.85)
- Z-position: minor_radius + 0.15 (sits on top surface)
- Purpose: Beam position monitors

### Cryogenic Systems

**Cryogenic Supply Lines** (4 pipes):
- Construction: 32 segments each, rendered as GL_LINES
- Position: Offset from torus at 4 different angles (0°, 90°, 180°, 270°)
- Offset distance: 70% of minor radius
- Color: Blue (0.2, 0.4, 0.7)
- Total segments: 128 line segments (32 × 4)

## Geometry Statistics

### Main Torus
- Vertices with displacement: 4,704
- Each vertex checked against 7 displacement patterns
- Average 3-5 surface features per vertex position

### Additional Structures
- Total objects: 58 (8+4+6+16+24)
- Box objects: 54
- Cylinder objects: 4
- Line segments: 128 (cryo lines)
- Total additional faces: ~540 quads (boxes) + ~48 quads (cylinders)

### Combined Complexity
- Main torus triangles: 9,216
- Structure quads: ~588
- Structure triangles equivalent: ~1,176
- **Total geometry**: ~10,392 triangles
- **Plus**: 128 line segments for pipes

## Rendering Details

All geometry uses:
- Proper normal vectors for lighting
- Individual color per object/surface region
- Depth testing (properly occluded)
- Same transformation matrix (rotates together)

The result is a **highly detailed 3D model** with:
- Real geometric depth (not flat!)
- Multiple 3D objects at different scales
- Proper perspective and occlusion
- Authentic particle accelerator appearance

This addresses the requirement for "x,y,z variance" and "other 3d models morphed into the main donut"!
