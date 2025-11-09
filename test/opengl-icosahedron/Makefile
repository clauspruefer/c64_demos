# Makefile for OpenGL Animated Icosahedron

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lGL -lGLU -lglut -lm

TARGET1 = opengl-icosahedron
SOURCE1 = opengl-icosahedron.cpp

TARGET2 = opengl-icosahedron-glpoints
SOURCE2 = opengl-icosahedron-glpoints.cpp

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(SOURCE1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(SOURCE1) $(LDFLAGS)

$(TARGET2): $(SOURCE2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(SOURCE2) $(LDFLAGS)

clean:
	rm -f $(TARGET1) $(TARGET2)

run: $(TARGET1)
	./$(TARGET1)

run-glpoints: $(TARGET2)
	./$(TARGET2)

.PHONY: all clean run run-glpoints
