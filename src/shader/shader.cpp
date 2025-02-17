#include <shader/shader.hpp>

#include <string>
#include <iostream>

#include <utilities/utilities.hpp>

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
    // Read files and store the contents.
    // Note: each pair of lines can't be combined, e.g., ReadFile(vertexFilePath).c_str(),
    // as the string goes out of scope after the function returns.
    std::string vertexCode = ReadFile(vertexFilePath);
    const char* vertexSource = vertexCode.c_str();
    std::string fragmentCode = ReadFile(fragmentFilePath);
	const char* fragmentSource = fragmentCode.c_str();

    // Variables for error checking
    int success;
    char infoLog[512];

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    CheckForCompilationErrors(vertexShader, "VERTEX");

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    CheckForCompilationErrors(fragmentShader, "FRAGMENT");

    // Create shader program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    CheckForCompilationErrors(programID, "PROGRAM");

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Do a final check to see if everything went well
    glCheckError();
}

void Shader::CheckForCompilationErrors(GLuint shader, const char* type) {
    int success;
    char infoLog[512]; 
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << TEXT_RED << "OpenGL Error: " << TEXT_YELLOW << "SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << TEXT_RESET << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << TEXT_RED << "OpenGL Error: " << TEXT_YELLOW << "SHADER::" << type << "::LINKING_FAILED\n" << infoLog << TEXT_RESET << std::endl;
        }
    }
}

void Shader::Activate() {
    glUseProgram(programID);
}

void Shader::Delete() {
    glDeleteProgram(programID);
}