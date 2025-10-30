# Makefile for OpenGL Cylinder Texture Demo (Linux only)

CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lGL -lGLU -lglut
TARGET = opengl-cylinder-texture
SRC = opengl-cylinder-texture.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
