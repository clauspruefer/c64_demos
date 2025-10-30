# Makefile for OpenGL Textured Cylinder

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lGL -lGLU -lglut -lm

TARGET = opengl-textured-cylinder
SOURCE = opengl-textured-cylinder.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
