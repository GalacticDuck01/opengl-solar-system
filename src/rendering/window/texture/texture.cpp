#include <Rendering/Window/Texture/Texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Utilities/Utilities.hpp>
#include <fstream>
#include <iostream>

/**
 * Constructor for a Texture object.
 *
 * \param image The path to the image file from which to load the texture.
 * \param textureType The type of texture to create (DIFFUSE or SPECULAR).
 * \param unit The texture unit on which to bind the texture. The texture unit is a number that allows multiple textures to be used at the same time.
 *
 * This function will check that the file exists and can be loaded. If not, an error message will be printed and the object will be in an invalid state.
 * The function will also generate mipmaps for the texture, and will bind the texture to the specified texture unit.
 */
Texture::Texture(const char* image, TextureType textureType, GLuint unit) {
    // Check image path exists
    ifstream f(image);
    if (!f.good()) {
        cout << TEXT_RED << "Error: " << TEXT_YELLOW << "Texture file does not exist: " << image << TEXT_RESET << endl;
        return;
    }
    path = image;
    type = textureType;
    int textureWidth, textureHeight, numColourChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image, &textureWidth, &textureHeight, &numColourChannels, 0);
    if (!data) {
        cout << TEXT_RED << "Error: " << TEXT_YELLOW << "Texture file could not be loaded: " << image << TEXT_RESET << endl;
        return;
    }

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + unit);
    this->unit = unit;
    glBindTexture(GL_TEXTURE_2D, id); // Bind the texture so we can adjust its parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Uses the texel nearest to the specified texture x coordinate
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Uses the texel nearest to the specified texture y coordinate
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT along the X axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Set texture wrapping to GL_REPEAT along the Y axis
    glCheckError();

    // Attach the image data to the currently bound texture (depending on the number of colour channels)
    if (numColourChannels == 4) { // RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else if (numColourChannels == 3) { // RGB
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (numColourChannels == 1) { // Grayscale
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    }
    else {
        cout << TEXT_RED << "Error: " << TEXT_YELLOW << "Invalid number of colour channels (expected 1, 3, or 4, but got " << numColourChannels << ")" << TEXT_RESET << endl;
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps (lower resolution copies of the texture for distance-based filtering)
    glCheckError();

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
}

/**
 * Sets the active texture unit for the specified uniform in the specified shader.
 *
 * \param shaderID The ID of the shader program to set the texture unit for.
 * \param uniform The name of the uniform variable to set the texture unit for.
 * \param unit The texture unit to set.
 *
 * This function will set the specified uniform to the specified texture unit using glUniform1i. It will also check for any OpenGL errors using glCheckError.
 */
void Texture::SetTextureUnit(unsigned int shaderID, const char* uniform, GLuint unit) {
    GLuint textureUniformID = glGetUniformLocation(shaderID, uniform);
    glUseProgram(shaderID);
    glUniform1i(textureUniformID, unit);
    glCheckError();
}

/**
 * Binds the texture to the specified texture unit.
 *
 * This function will use glActiveTexture to set the active texture unit to the unit specified in the constructor, and then use glBindTexture to bind the texture to that unit.
 */
void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

/**
 * Unbinds the texture from the active texture unit.
 *
 * This function will reset the active texture unit to 0 (i.e. the default texture unit).
 * It is useful for unbinding a texture after it has been used for rendering.
 */
void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Deletes the texture and frees up the OpenGL resources it was using.
 *
 * This function will call glDeleteTextures to delete the texture and free up the OpenGL resources it was using.
 * It should be called when the texture is no longer needed in order to avoid memory leaks.
 */
void Texture::Delete() {
    glDeleteTextures(1, &id);
}