# Makefile for Mandelbrot project using traditional headers

CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2

# Source files
SRC = Mandelbrot.cpp main.cpp
HEADERS = Mandelbrot.h

# Object files
OBJ = Mandelbrot.o Bitmap.o main.o

# Executable
TARGET = fractal

all: $(TARGET)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
