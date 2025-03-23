#pragma once

#include <Rendering/Window/Window.hpp>
#include <Camera/Camera.hpp>
#include <Simulation/Icosphere/Icosphere.hpp>

#include <glm/glm.hpp>
#include <map>

using namespace glm;

class Simulation {
    private:
        Window window{WIDTH, HEIGHT, "Solar System Simulation"};
        Camera camera{WIDTH, HEIGHT, vec3(0.0f, 0.0f, 2.0f)};
        double previousTime = 0.0f;
        double currentTime = 0.0f;
        double timeSinceFPSUpdate = 0.0f;

        std::map<int, Shader> shaders;
        std::map<int, std::vector<Mesh>> drawableObjects;

        void update(float deltaTime);
        void render();
        int loadShader(const char* vertexFilePath, const char* fragmentFilePath);
        void addDrawable(Icosphere icosphere);
    public:
        static const int WIDTH = 1920;
        static const int HEIGHT = 1000;

        void Run();
};