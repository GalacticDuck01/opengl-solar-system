#pragma once

#include <map>
#include <glad/glad.h>
#include <shader/shader.hpp>

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class Texture {
    public:
        GLuint id;
        TextureType type;
        GLuint unit;
        
        Texture(const char* image, TextureType textureType, GLuint slot);

        const char* GetTextureTypeAsString() { return textureTypeToString[type]; }

        void TexUnit(Shader& shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();

    private:
        // Textures will be named "diffuse0", "diffuse1", "specular0", "specular1", etc.
        std::map <TextureType, const char*> textureTypeToString = {
            {TextureType::DIFFUSE, "diffuse"},
            {TextureType::SPECULAR, "specular"}
        };
};