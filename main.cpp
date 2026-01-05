#include "Mandelbrot.h"
#include "Bitmap.h"

#include <iostream>
#include <cstdint>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>


using highres_clock = std::chrono::high_resolution_clock;


// Function to render via one single thread
void render_single(
    Bitmap& bitmap,
    int WIDTH,
    int HEIGHT,
    double X_MIN,
    double Y_MIN,
    double X_SCALE,
    double Y_SCALE
) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double xFractal = X_MIN + x * X_SCALE;
            double yFractal = Y_MIN + y * Y_SCALE;

            int iterations = Mandelbrot::getIterations(xFractal, yFractal);

            uint8_t r, g, b;
            if (iterations == Mandelbrot::MAX_ITERATIONS) {
                r = g = b = 0;
            } else {
                double t = log(1.0 + iterations) / log(1.0 + Mandelbrot::MAX_ITERATIONS);
                r = static_cast<uint8_t>(sin(0.016 * iterations + 4) * 230 + 25);
                g = static_cast<uint8_t>(sin(0.013 * iterations + 2) * 230 + 25);
                b = static_cast<uint8_t>(sin(0.01 * iterations + 1) * 230 + 25);
            }
            
            bitmap.setPixel(x, y, r, g, b);
        }
    }
}

// Function to render via multiple threads. 
void render_worker(
    Bitmap& bitmap,
    std::atomic<int>& nextRow,
    int WIDTH,
    int HEIGHT,
    double X_MIN,
    double Y_MIN,
    double X_SCALE,
    double Y_SCALE
) {
    int y;
    while ((y = nextRow.fetch_add(1)) < HEIGHT) {
        for (int x = 0; x < WIDTH; x++) {
            double xFractal = X_MIN + x * X_SCALE;
            double yFractal = Y_MIN + y * Y_SCALE;

            int iterations = Mandelbrot::getIterations(xFractal, yFractal);

            uint8_t r, g, b;
            if (iterations == Mandelbrot::MAX_ITERATIONS) {
                r = g = b = 0; // inside black
            } else {
                double t = log(1.0 + iterations) / log(1.0 + Mandelbrot::MAX_ITERATIONS);
                r = static_cast<uint8_t>(sin(0.016 * iterations + 4) * 230 + 25);
                g = static_cast<uint8_t>(sin(0.013 * iterations + 2) * 230 + 25);
                b = static_cast<uint8_t>(sin(0.01 * iterations + 1) * 230 + 25);
            }
            
            bitmap.setPixel(x, y, r, g, b);
        }
    }
}

int main() {
    const int WIDTH  = 5000;
    const int HEIGHT = 5000;

    const double X_MIN = -2.5;
    const double X_MAX =  1.5;
    const double Y_MIN = -2.0;
    const double Y_MAX =  2.0;

    const double X_SCALE = (X_MAX - X_MIN) / WIDTH;
    const double Y_SCALE = (Y_MAX - Y_MIN) / HEIGHT;

    std::cout << "Resolution: " << WIDTH << "x" << HEIGHT << std::endl;
    std::cout << "Iterations: " << Mandelbrot::MAX_ITERATIONS << std::endl;

    // Normal fractal generation
    {
        Bitmap bitmap(WIDTH, HEIGHT);

        auto start = highres_clock::now();
        render_single(bitmap, WIDTH, HEIGHT,
                      X_MIN, Y_MIN, X_SCALE, Y_SCALE);
        auto end = highres_clock::now();

        double seconds =
            std::chrono::duration<double>(end - start).count();

        std::cout << "[Single-std::threaded] Time: "
             << seconds << " seconds" << std::endl;

        bitmap.write("mandelbrot_single.bmp");
    }

    // Multithreaded fractal generation
    {
        Bitmap bitmap(WIDTH, HEIGHT);

        unsigned int threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) threadCount = 4;

        std::cout << "Using " << threadCount << " threads" << std::endl;

        std::atomic<int> nextRow{0};
        std::vector<std::thread> threads;

        auto start = highres_clock::now();

        for (unsigned int i = 0; i < threadCount; i++) {
            threads.emplace_back(
                render_worker,
                std::ref(bitmap),
                std::ref(nextRow),
                WIDTH,
                HEIGHT,
                X_MIN,
                Y_MIN,
                X_SCALE,
                Y_SCALE
            );
        }

        for (auto& t : threads) {
            t.join();
        }

        auto end = highres_clock::now();

        double seconds =
            std::chrono::duration<double>(end - start).count();

        std::cout << "[Multi-threaded] Time: "
             << seconds << " seconds" << std::endl;

        bitmap.write("mandelbrot_threaded.bmp");
    }

    return 0;
}