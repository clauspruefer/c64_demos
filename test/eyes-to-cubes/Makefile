# Makefile for Eyes to Cubes Animation

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lGL -lGLU -lglut -lm

TARGET = eyes-to-cubes
SOURCE = eyes-to-cubes.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
