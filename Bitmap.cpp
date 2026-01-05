#include "Bitmap.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

Bitmap::Bitmap(int width, int height) 
    : width(width), height(height) {
    // Allocate memory for RGB pixels (3 bytes per pixel)
    pixels = new uint8_t[width * height * 3];
    
    // Initialize to black
    memset(pixels, 0, width * height * 3);
}

Bitmap::~Bitmap() {
    delete[] pixels;
}

int Bitmap::getPixelIndex(int x, int y) const {
    return (x * height + y) * 3;
}

void Bitmap::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return; // Out of bounds
    }
    
    int index = getPixelIndex(x, y);
    pixels[index] = red;
    pixels[index + 1] = green;
    pixels[index + 2] = blue;
}

bool Bitmap::write(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    // Calculate row size (must be multiple of 4 bytes for BMP)
    int rowSize = ((width * 3 + 3) / 4) * 4; // Pad to 4-byte boundary
    int pixelDataSize = rowSize * height;
    int fileSize = 14 + 40 + pixelDataSize; // File header + Info header + pixel data
    
    // Write BMP file header (14 bytes)
    file.put('B');
    file.put('M');
    // File size (little-endian)
    file.put(static_cast<char>(fileSize & 0xFF));
    file.put(static_cast<char>((fileSize >> 8) & 0xFF));
    file.put(static_cast<char>((fileSize >> 16) & 0xFF));
    file.put(static_cast<char>((fileSize >> 24) & 0xFF));
    // Reserved
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    // Offset to pixel data (14 + 40 = 54)
    file.put(54);
    file.put(0);
    file.put(0);
    file.put(0);
    
    // Write BMP info header (40 bytes)
    // Header size
    file.put(40);
    file.put(0);
    file.put(0);
    file.put(0);
    // Width (little-endian)
    file.put(static_cast<char>(width & 0xFF));
    file.put(static_cast<char>((width >> 8) & 0xFF));
    file.put(static_cast<char>((width >> 16) & 0xFF));
    file.put(static_cast<char>((width >> 24) & 0xFF));
    // Height (little-endian, positive = bottom-up)
    file.put(static_cast<char>(height & 0xFF));
    file.put(static_cast<char>((height >> 8) & 0xFF));
    file.put(static_cast<char>((height >> 16) & 0xFF));
    file.put(static_cast<char>((height >> 24) & 0xFF));
    // Planes
    file.put(1);
    file.put(0);
    // Bits per pixel
    file.put(24);
    file.put(0);
    // Compression (0 = none)
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    // Image size
    file.put(static_cast<char>(pixelDataSize & 0xFF));
    file.put(static_cast<char>((pixelDataSize >> 8) & 0xFF));
    file.put(static_cast<char>((pixelDataSize >> 16) & 0xFF));
    file.put(static_cast<char>((pixelDataSize >> 24) & 0xFF));
    // X pixels per meter (0 = unused)
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    // Y pixels per meter (0 = unused)
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    // Colors used (0 = all)
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    // Important colors (0 = all)
    file.put(0);
    file.put(0);
    file.put(0);
    file.put(0);
    
    // Write pixel data (BMP is stored bottom-up, so we need to reverse rows)
    // Also BMP uses BGR format, not RGB
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int index = getPixelIndex(x, y);
            // Write BGR
            file.put(static_cast<char>(pixels[index + 2])); // Blue
            file.put(static_cast<char>(pixels[index + 1])); // Green
            file.put(static_cast<char>(pixels[index]));     // Red
        }
        // Pad row to 4-byte boundary
        for (int p = width * 3; p < rowSize; p++) {
            file.put(0);
        }
    }
    
    if (!file.good()) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        file.close();
        return false;
    }
    
    file.close();
    std::cout << "Bitmap written to " << filename << std::endl;
    return true;
}



