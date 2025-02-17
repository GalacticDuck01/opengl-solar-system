#include <rendering/window/VBO/VBO.hpp>

#include <utilities/utilities.hpp>

VBO::VBO(std::vector<Vertex> vertices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glCheckError();
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glCheckError();
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
}

void VBO::Delete() { 
    glDeleteBuffers(1, &ID);
    glCheckError();
}