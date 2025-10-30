# OpenGL Textured Cylinder Demo

Enhanced version of the OpenGL rotating cylinder with double helix, adding a textured cylinder surface between the two helix ropes.

## Features

- Rotating cylinder with texture mapping
- Vertical text on the cylinder surface: "this is a cylinder opengl demo by twf/hitmen"
- Double helix ropes winding around the textured cylinder
- Text appears from bottom to top (one character under each other)

## Building

```bash
make
```

## Running

```bash
make run
# or
./opengl-textured-cylinder
```

## Requirements

- OpenGL libraries (GL, GLU, GLUT)
- GLM library (for vector math)
- C++11 compatible compiler

On Ubuntu/Debian:
```bash
sudo apt-get install freeglut3-dev libglm-dev
```

## Description

This demo creates a 3D rotating cylinder with a blue textured surface. The texture displays the text "this is a cylinder opengl demo by twf/hitmen" vertically (from bottom to top). Two intertwined helix ropes wind around the cylinder, creating a double helix effect similar to DNA strands.

The cylinder rotates continuously, allowing you to see the text from all angles as it wraps around the cylindrical surface.
