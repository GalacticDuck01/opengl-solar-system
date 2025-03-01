#pragma once

#include <Rendering/Window/Window.hpp>

class Simulation {
    private:
        Window window{WIDTH, HEIGHT, "Solar System Simulation"};
    public:
        static const int WIDTH = 1000;
        static const int HEIGHT = 1000;

        void Run();
};