cmake_minimum_required(VERSION 3.10)

project(SolarSystem)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(submodules/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

include_directories(submodules/glad/include/
                    submodules/glfw/include/
                    submodules/glm/
                    submodules/stb/
                    submodules/assimp/include/
                    submodules/assimp/build/include/)

# Set bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set include directory
set(INC_DIR include)

# Set source directory
set(SRC_DIR src)

# Recursively find all source files
file(GLOB_RECURSE CPP_SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE C_SOURCES "${SRC_DIR}/*.c")

# Recursively find all header files
file(GLOB_RECURSE HPP_HEADERS "${SRC_DIR}/*.hpp")

# Find all header files in include directorys
file(GLOB_RECURSE INC_HEADERS "${INC_DIR}/*.h")

# Find required dependency files
file(GLOB DEP_SOURCES submodules/glad/src/glad.c)

# Add all source files to the project
include_directories(${SRC_DIR})

# Add all header files to the project
include_directories(${INC_DIR})

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${C_SOURCES} ${DEP_SOURCES} ${HPP_HEADERS} ${H_HEADERS} ${INC_HEADERS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} assimp)
