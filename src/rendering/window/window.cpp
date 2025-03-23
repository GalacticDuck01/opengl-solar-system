#include <Rendering/Window/Window.hpp>

#include <iostream>
#include <tuple>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Utilities/Utilities.hpp>

Window::Window(int width, int height, std::string name) {
    this->width = width;
    this->height = height;
    this->name = name;
    InitWindow();
}

/**
 * Destroys the window by calling glfwDestroyWindow and then terminates the GLFW library.
 */
Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * Initializes the GLFW window.
 * 
 * This function sets up the window using GLFW, disabling the OpenGL API
 * and making the window non-resizable. It creates a window with the
 * specified width, height, and name.
 * 
 * @throws std::runtime_error If the GLFW library fails to initialize.
 */
void Window::InitWindow() {
    glfwInit(); // Initialize the GLFW library
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Use OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile
    glfwWindowHint(GLFW_SAMPLES, 4); // Multisample anti-aliasing (MSSA), use 4 samples

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); // Create the GLFW window

    if (window == NULL) {
        std::cout << "Error: Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->CallbackFrameworkSize); // Set the callback function for framebuffer size changes

    gladLoadGL();

    // GLAD: Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error: Failed to initialize GLAD" << std::endl;
    }

    // To ensure the correct ordering of textures
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    // Turn on VSync
    glfwSwapInterval(1);

    // Final check for errors
    glCheckError();
}

/**
 * @brief Callback function for window resize events.
 * 
 * (GLFW) Whenever the window size changes (by the user resizing the window or
 * because the video mode changed) this callback function executes.
 *
 * @param window The window that received the event
 * @param width The new width, in screen coordinates, of the window
 * @param height The new height, in screen coordinates, of the window
 *
 * @remarks We use the new window dimensions to update the OpenGL viewport.
 */
void Window::CallbackFrameworkSize(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    glCheckError();
}

/**
 * @brief Callback function for cursor enter/exit events.
 * 
 * (GLFW) Whenever the cursor enters or leaves the content area of the window this callback function executes
 *
 * @param window The window that received the event
 * @param entered GLFW_TRUE if the cursor entered the content area of the window, otherwise GLFW_FALSE
 */
void Window::CallbackCursorEnter(GLFWwindow* window, int entered) {
    if (entered) {
        // The cursor entered the content area of the window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        // The cursor left the content area of the window
    }
}