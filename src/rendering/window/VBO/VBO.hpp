#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 colour;
    glm::vec2 textureCoords;
};

class VBO {
    public:
        GLuint ID;

        VBO(std::vector<Vertex> vertices);

        void Bind();
        void Unbind();
        void Delete();
};