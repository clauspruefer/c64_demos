# Makefile for OpenGL Donut Particle Accelerator

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lGL -lGLU -lglut -lm

TARGET = donut-accelerator
SOURCE = opengl-textured-rotating-donut.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
