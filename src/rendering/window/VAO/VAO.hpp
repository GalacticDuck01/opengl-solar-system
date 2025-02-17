#pragma once

#include <glad/glad.h>

#include <rendering/window/VBO/VBO.hpp>

class VAO {
    public:
        GLuint ID;

        VAO();
        void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void Delete();
};