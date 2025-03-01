#include <Camera/Camera.hpp>

Camera::Camera(int width, int height, glm::vec3 position) {
    this->width = width;
    this->height = height;
    this->position = position;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(position, position + orientation, up);

    float tanFov = tan(0.5f * FOVdeg*3.14159f/180.0f);
    float oneTanFov = 1.0f / tanFov;
    float aspRat = (float)width / (float)height;
    float oneTanAspRat = 1.0f/(tanFov*aspRat);

    projection[0][0] = 1.0f / (aspRat * tanFov);
    projection[0][1] = 0.0f;
    projection[0][2] = 0.0f;
    projection[0][3] = 0.0f;

    projection[1][0] = 0.0f;
    projection[1][1] = 1.0f / tanFov;
    projection[1][2] = 0.0f;
    projection[1][3] = 0.0f;

    projection[2][0] = 0.0f;
    projection[2][1] = 0.0f;
    projection[2][2] = (nearPlane + farPlane) / (nearPlane - farPlane);
    projection[2][3] = -1.0f;

    projection[3][0] = 0.0f;
    projection[3][1] = 0.0f;
    projection[3][2] = 2.0f * nearPlane * farPlane / (nearPlane - farPlane);
    projection[3][3] = 0.0f;


    cameraMatrix = projection*view;
}

/**
 * Send the camera matrix to the given shader's uniform.
 *
 * @param shader The shader to send the matrix to.
 * @param uniform The name of the uniform in the shader.
 */
void Camera::SendMatrixToShader(Shader& shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.programID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::HandleInputs(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed*orientation*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= speed*glm::normalize(glm::cross(orientation, up))*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= speed*orientation*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += speed*glm::normalize(glm::cross(orientation, up))*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += speed*up*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position -= speed*up*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
        speed = 5.0f;
    }

    // Mouse controls
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (isFirstClick) {
            glfwSetCursorPos(window, width/2, height/2);
            isFirstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotationX = sensitivity*(float)(mouseY - height/2)/height;
        float rotationY = sensitivity*(float)(mouseX - width/2)/width;

        glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotationX), glm::normalize(glm::cross(orientation, up)));
        if (!((glm::angle(newOrientation, up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -up) <= glm::radians(5.0f)))) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-rotationY), up);

        // Set mouse position to the centre of the screen
        glfwSetCursorPos(window, width/2, height/2);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        isFirstClick = true;
    }
}