#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Camera/Camera.hpp>
#include <Rendering/Window/Mesh/Mesh.hpp>

class Window {
    private:
        std::string name;

        void initWindow();

        static void callbackCursorEnter(GLFWwindow* window, int entered);
        static void callbackFrameworkSize(GLFWwindow* window, int width, int height);

    public:
        GLFWwindow* window;
        int width;
        int height;
        bool hasCursorEntered = false;

        Window(int width, int height, std::string name);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool ShouldClose() { return glfwWindowShouldClose(window); }
};