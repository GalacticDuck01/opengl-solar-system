#include <simulation/simulation.hpp>

#include <iostream>
#include <math.h>
#include <vector>

#include <rendering/window/model/model.hpp>
#include <rendering/window/texture/texture.hpp>
#include <rendering/window/VBO/VBO.hpp>

/**
 * @brief Runs the main loop for the simulation.
 *
 * This continues to run until the user closes the window.
 * It is responsible for processing any user input to the window.
 */
void Simulation::Run() {
    Shader shaderProgram("C:/Users/samru/Desktop/code/opengl-work-in-progress/shaders/default.vert", "C:/Users/samru/Desktop/code/opengl-work-in-progress/shaders/default.frag");

    glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    Model bunnyModel = Model("C:/Users/samru/Desktop/code/opengl-work-in-progress/resources/models/sword/scene.gltf");

    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.programID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
    glUniform3f(glGetUniformLocation(shaderProgram.programID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Camera camera(window.width, window.height, glm::vec3(0.0f, 0.0f, 2.0f));

    double previousTime = 0.0f;
    double currentTime = 0.0f;
    double deltaTime = 0.0f;
    double timeSinceFPSUpdate = 0.0f;
    int FPS = 0;
    std::string title;

    // Main loop
    while (!window.ShouldClose()) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        timeSinceFPSUpdate += deltaTime;
        if (timeSinceFPSUpdate >= 1.0f) {
            FPS = (int)(1.0f/deltaTime);
            title = "OpenGL Window - FPS: " + std::to_string(FPS);
            glfwSetWindowTitle(window.window, title.c_str());
            timeSinceFPSUpdate = 0.0f;
        }
        previousTime = currentTime;

        camera.HandleInputs(window.window, deltaTime);
        
        window.Render(camera, bunnyModel, shaderProgram);
    }

    shaderProgram.Delete();
}