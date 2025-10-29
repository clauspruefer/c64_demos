# Makefile for OpenGL Cylinder Helix Demo (Linux only)

CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lGL -lGLU -lglut
TARGET = opengl-cylinder-helix
SRC = opengl-cylinder-helix.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
