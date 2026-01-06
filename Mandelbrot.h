class Mandelbrot {
    public:
        Mandelbrot() = delete;
        ~Mandelbrot() = delete;
        static int getIterations(double x, double y);
        static const int MAX_ITERATIONS = 5000;
    private:
};