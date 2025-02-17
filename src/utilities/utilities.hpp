#pragma once

#include <string>
#include <glad/glad.h>
#include <utilities/colour_output.hpp>

#define TEXT_RESET      Modifier(FG_DEFAULT)
#define TEXT_RED        Modifier(FG_RED)
#define TEXT_GREEN      Modifier(FG_GREEN)
#define TEXT_BLUE       Modifier(FG_BLUE)
#define TEXT_YELLOW     Modifier(FG_YELLOW)
#define TEXT_MAGENTA    Modifier(FG_MAGENTA)
#define TEXT_CYAN       Modifier(FG_CYAN)
#define TEXT_WHITE      Modifier(FG_WHITE)

std::string ReadFile(const std::string& filename);

GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)