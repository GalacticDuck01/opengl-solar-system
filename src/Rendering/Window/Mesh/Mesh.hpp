#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <Rendering/Window/Texture/Texture.hpp>
#include <Camera/Camera.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 colour;
    glm::vec2 textureCoords;
};

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        Mesh() {};
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void Draw(Shader& shader, Camera& camera, glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};