#include "Mandelbrot.h"


Mandelbrot::Mandelbrot() {
}

Mandelbrot::~Mandelbrot() {
}

int Mandelbrot::getIterations(double x, double y) {  // static method
    
    int iterations = 0;
    double zx = 0;
    double zy = 0;
    while (zx*zx + zy*zy <= 4 && iterations < Mandelbrot::MAX_ITERATIONS) {
        double xt = zx*zx - zy*zy + x;
        zy = 2*zx*zy + y;
        zx = xt;
        iterations++;
    }
    
    return iterations;
}
