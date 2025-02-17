#include <rendering/window/EBO/EBO.hpp>
#include <utilities/utilities.hpp>

EBO::EBO(std::vector<unsigned int> indices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glCheckError();
}

void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glCheckError();
}

void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
}

void EBO::Delete() { 
    glDeleteBuffers(1, &ID);
    glCheckError();
}