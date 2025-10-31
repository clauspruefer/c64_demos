# Makefile for Alien Animation Generator

.PHONY: all clean install run help

# Python interpreter
PYTHON = python3

# Output file
OUTPUT = alien_animation.mp4

all: install run

install:
	@echo "Installing Python dependencies..."
	$(PYTHON) -m pip install --user moviepy pillow numpy
	@echo "Dependencies installed."
	@echo ""
	@echo "Note: This tool requires ffmpeg to be installed on your system."
	@echo "If you don't have ffmpeg, install it with:"
	@echo "  Ubuntu/Debian: sudo apt-get install ffmpeg"
	@echo "  macOS: brew install ffmpeg"
	@echo ""

run:
	@echo "Generating alien animation..."
	$(PYTHON) generate_alien_animation.py

clean:
	@echo "Cleaning up generated files..."
	rm -f $(OUTPUT)
	rm -rf __pycache__
	@echo "Clean complete."

help:
	@echo "Alien Animation Generator - Makefile targets:"
	@echo ""
	@echo "  make install  - Install required Python dependencies"
	@echo "  make run      - Generate the animation"
	@echo "  make all      - Install dependencies and generate animation"
	@echo "  make clean    - Remove generated files"
	@echo "  make help     - Show this help message"
	@echo ""
