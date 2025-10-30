# Makefile for Mandelbrot Zoom Animation Generator

CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11
LDFLAGS = -lpng -lm
TARGET = generate_mandelbrot_zoom
SRC = generate_mandelbrot_zoom.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -rf frames/

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
