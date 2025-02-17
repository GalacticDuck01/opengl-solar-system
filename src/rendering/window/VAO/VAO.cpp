#include <rendering/window/VAO/VAO.hpp>

#include <utilities/utilities.hpp>

#include <iostream>


/**
 * @brief Default constructor that generates a VAO ID
 *
 * This function generates a VAO ID and stores it in the ID member variable.
 * It also checks for any OpenGL errors that this may cause.
 */

VAO::VAO() {
    glGenVertexArrays(1, &ID);
    glCheckError();
}

/**
 * @brief Links a VBO Attribute to the VAO
 *
 * @param VBO The VBO containing the attribute data
 * @param layout The attribute layout location
 * @param numComponents The number of components for this attribute
 * @param type The type of data for each component
 * @param stride The stride between consecutive elements in the attribute array
 * @param offset The offset of the attribute data in the VBO
 */
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    glCheckError();
    VBO.Unbind();
}

/**
 * @brief Binds the VAO
 *
 * This function binds the VAO to the current OpenGL context, allowing
 * it to be used for rendering. It also checks for any OpenGL errors
 * that this may cause.
 */
void VAO::Bind() {
    glBindVertexArray(ID);
    glCheckError();
}

/**
 * @brief Unbinds the VAO
 *
 * This function unbinds the VAO from the current OpenGL context, allowing
 * another VAO to be used for rendering. It also checks for any OpenGL errors
 * that this may cause.
 */
void VAO::Unbind() {
    glBindVertexArray(0);
    glCheckError();
}

/**
 * @brief Deletes the VAO
 *
 * This function deletes the VAO from the current OpenGL context, freeing
 * up any resources that it may have been using. It also checks for any OpenGL
 * errors that this may cause.
 */
void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
    glCheckError();
}