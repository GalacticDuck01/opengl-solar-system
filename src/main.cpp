#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <Simulation/Simulation.hpp>

/**
 * @brief Entry point of the program.
 * 
 * Initializes the OpenGL context and GLFW, sets up the window and
 * rendering loop, and handles input and window resizing.
 * 
 * @return int Exit status of the program.
 */
int main() {
    Simulation simulation;

    try {
        simulation.Run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}