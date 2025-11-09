# 3D Cube Animation Demo

A visually appealing OpenGL demonstration featuring a smoothly rotating 3D cube with dynamic color transitions.

## Features

- **Smooth 3D Rotation**: The cube rotates on all three axes with organic, varying speeds
- **Dynamic Color Transitions**: Each face of the cube smoothly cycles through vibrant colors
- **Edge Highlighting**: White edges provide clear definition of the cube structure
- **Fluid Animation**: 60 FPS rendering for smooth visual experience

## Technical Details

- **Window Size**: 800×600 pixels
- **Frame Rate**: 60 FPS
- **Rendering**: Double-buffered with depth testing
- **Animation**: Time-based color and rotation updates using sine waves for smooth transitions
- **Colors**: Each face independently transitions through the RGB color spectrum

## Building

### Requirements
- g++ compiler with C++11 support
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)

### On Ubuntu/Debian:
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
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
./3d-cube-animation
```

## How It Works

The animation uses several techniques to create a visually interesting display:

1. **Multi-Axis Rotation**: The cube rotates simultaneously around X, Y, and Z axes with speeds that vary smoothly using sine wave modulation
2. **Color Cycling**: Each of the six faces cycles through colors independently, creating a rainbow effect
3. **Smooth Transitions**: All animations use continuous mathematical functions (sine waves) rather than discrete steps
4. **Phase Shifting**: Each face's color is offset by 60 degrees in the color cycle for visual variety

## Implementation Notes

- Uses OpenGL immediate mode for simplicity and compatibility
- Depth testing ensures proper face rendering order
- Double buffering eliminates flickering
- Timer-based animation ensures consistent frame rate

Enjoy the mesmerizing 3D cube animation!
