#!/bin/bash
# Build script for Alien Fart demo

echo "Building Alien Fart demo..."
acme -f cbm -o alien-fart.prg main.asm

if [ $? -eq 0 ]; then
    echo "Build successful! Created alien-fart.prg"
    echo "Run with: x64 alien-fart.prg"
    echo "Or autostart: x64 -autostart alien-fart.prg"
else
    echo "Build failed!"
    exit 1
fi
