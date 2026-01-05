#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <string>

class Bitmap {
    public:
        Bitmap(int width, int height);
        ~Bitmap();
        
        // Set pixel color at (x, y)
        void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
        
        // Write bitmap to file (BMP format)
        bool write(const std::string& filename);
        
        // Get dimensions
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        
    private:
        int width;
        int height;
        uint8_t* pixels;
        
        // Helper to calculate pixel index
        int getPixelIndex(int x, int y) const;
};

#endif // BITMAP_H

