#include <Simulation/Simulation.hpp>

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

#include <Rendering/Window/Texture/Texture.hpp>
#include <Simulation/Icosphere/Icosphere.hpp>
#include <Utilities/Utilities.hpp>

/**
 * @brief Runs the main loop for the simulation.
 *
 * This continues to run until the user closes the window.
 * It is responsible for processing any user input to the window.
 */
void Simulation::Run() {
    int shaderProgram = loadShader("shaders/default.vert", "shaders/default.frag");
    Icosphere icosphere(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 5);
    icosphere.SetShader(shaderProgram);
    addDrawable(icosphere);

    glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    Shader shader = shaders.at(shaderProgram);
    shader.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Main loop
    while (!window.ShouldClose()) {
        currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        update(deltaTime);
        render();
    }

    shader.Delete();
}

/**
 * @brief Updates the simulation state based on the elapsed time since the last frame.
 *
 * This handles any user input to the window, updates the camera's position and orientation based on input, and
 * checks if the window has changed size.
 *
 * @param deltaTime the time since the last frame.
 */
void Simulation::update(float deltaTime) {
    timeSinceFPSUpdate += deltaTime;
    if (timeSinceFPSUpdate >= 1.0f) {
        int FPS = (int)(1.0f/deltaTime);
        std::string title = "Solar System Simulation | " + std::to_string(window.width) + " x " + std::to_string(window.height) + " | FPS: " + std::to_string(FPS);
        glfwSetWindowTitle(window.window, title.c_str());
        timeSinceFPSUpdate = 0.0f;
    }
    previousTime = currentTime;

    camera.HandleInputs(window.window, deltaTime);

    // Check if the window has changed size
    glfwGetFramebufferSize(window.window, &window.width, &window.height);
    glfwGetFramebufferSize(window.window, &camera.width, &camera.height);
    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
}

/**
 * @brief Renders the scene.
 *
 * This sets the clear color and clears the color and depth buffers, updates the camera's matrix, and renders each mesh in the scene.
 * Finally, it swaps the front and back buffers and processes any pending events.
 */
void Simulation::render() {
    glClearColor(0.71f, 0.90f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError();

    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

    for (auto& mesh : drawableObjects) {
        int shaderID = mesh.first;
        std::vector<Mesh>* meshVector = &mesh.second;

        // for (auto& mesh : *meshVector) {
        //     mesh.Draw(shaders.at(shaderID), camera);
        // }

        backpack.Draw(shaders.at(shaderID), camera);
    }

    glfwSwapBuffers(window.window);
    glCheckError();

    glfwPollEvents();
}

/**
 * @brief Loads a shader from file and adds it to the shaders map.
 *
 * Creates a Shader object from the given vertex and fragment shader files, and
 * adds it to the shaders map, keyed by the Shader's program ID.
 *
 * @param vertexFilePath the path to the vertex shader file.
 * @param fragmentFilePath the path to the fragment shader file.
 * @return the Shader's program ID.
 */
int Simulation::loadShader(const char* vertexFilePath, const char* fragmentFilePath) {
    Shader shaderProgram(vertexFilePath, fragmentFilePath);
    int id = shaderProgram.programID;
    shaders.insert(std::pair<int, Shader>(id, shaderProgram));

    return id;
}

/**
 * @brief Adds an Icosphere to the simulation's drawable meshes.
 *
 * This function retrieves the shader ID from the given Icosphere
 * and checks if the shader ID is already a key in the meshes map.
 * If not, it initializes an empty vector of Meshes for that shader ID.
 * The Icosphere's mesh is then added to the vector associated with the shader ID.
 *
 * @param icosphere The Icosphere object to be added to the drawable meshes.
 */
void Simulation::addDrawable(Icosphere icosphere) {
    int id = icosphere.GetShader();
    // if shader ID not in keys, add it
    if (drawableObjects.find(id) == drawableObjects.end()) {
        drawableObjects.insert(std::pair<int, std::vector<Mesh>>(id, std::vector<Mesh>()));
    }

    std::vector<Mesh> meshVector = drawableObjects[id];
    meshVector.push_back(icosphere.mesh);
    drawableObjects[id] = meshVector;
}