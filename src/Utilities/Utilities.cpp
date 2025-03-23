#include <Utilities/Utilities.hpp>

#include <fstream>
#include <iostream>

/**
 * Reads the contents of a file and returns a pointer to it.
 *
 * \param filename The name of the file to read.
 * \return A pointer to the contents of the file or nullptr if the file does not exist.
 */
std::string ReadFile(const std::string& filename) {
    // Check if file exists
    std::ifstream f(filename);
    if (!f.good()) {
        outputError("File does not exist: " + filename);
        return nullptr;
    }

    std::string contents;
    f.seekg(0, std::ios::end);
    contents.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(&contents[0], contents.size());
    f.close();
    return contents;
}

/**
 * \brief Checks for OpenGL errors and prints them to the console if they are encountered.
 * \param file The file that the error occurred in.
 * \param line The line that the error occurred on.
 * \return The error code if an error occurred, otherwise GL_NO_ERROR.
 */
GLenum glCheckError_(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            default:                               error = "UNKNOWN_ERROR"; break;
        }
        std::cout << TEXT_RED << "OpenGL Error: " << TEXT_YELLOW << error << TEXT_RESET << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

void outputError_(const std::string& message, const char* file, int line) {
    std::cout << TEXT_RED << "Error: " << TEXT_YELLOW << message << TEXT_RESET << " | " << file << " (" << line << ")" << std::endl;
}