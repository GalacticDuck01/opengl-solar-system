#pragma once

#include <Rendering/Window/Window.hpp>
#include <Camera/Camera.hpp>

#include <glm/glm.hpp>

using namespace glm;

class Simulation {
    private:
        Window window{WIDTH, HEIGHT, "Solar System Simulation"};
        Camera camera{WIDTH, HEIGHT, vec3(0.0f, 0.0f, 2.0f)};
    public:
        static const int WIDTH = 1920;
        static const int HEIGHT = 1000;

        void Run();
};