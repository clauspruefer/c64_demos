# Makefile for Three Spheres OpenGL Display

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lGL -lGLU -lglut -lm

TARGET = three-spheres
SOURCE = three-spheres.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
