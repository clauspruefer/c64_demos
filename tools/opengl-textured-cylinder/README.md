# OpenGL Textured Double Helix Demo

Enhanced version of the OpenGL rotating double helix demo, adding a textured surface between the two helix strands.

## Features

- Textured surface connecting the two helix strands
- Vertical text on the helix surface: "this is a cylinder opengl demo by twf/hitmen"
- Text appears from bottom to top (one character under each other)
- Helix points remain visible on top of the textured surface
- Rotating 3D visualization

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

This demo creates a rotating 3D double helix structure with a textured surface connecting the two intertwined strands. The texture displays the text "this is a cylinder opengl demo by twf/hitmen" vertically (from bottom to top), mapped onto quads that bridge between the two helix strands. The original helix points remain visible on top of the textured surface, maintaining the characteristic double helix visualization while adding the textured surface between them.

The structure rotates continuously, allowing you to see the text from all angles as it wraps around with the helix.
