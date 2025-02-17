#pragma once

#include <vector>
#include <glad/glad.h>

class EBO {
    public:
        GLuint ID;

        EBO(std::vector<unsigned int> indices);

        void Bind();
        void Unbind();
        void Delete();
};