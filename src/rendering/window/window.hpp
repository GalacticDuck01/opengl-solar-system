#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera/camera.hpp>
#include <rendering/window/model/model.hpp>

class Window {
    private:
        std::string name;

        void InitWindow();

        static void CallbackCursorEnter(GLFWwindow* window, int entered);
        static void CallbackFrameworkSize(GLFWwindow* window, int width, int height);

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
        void Render(Camera& camera, Model& model, Shader& shader);
};