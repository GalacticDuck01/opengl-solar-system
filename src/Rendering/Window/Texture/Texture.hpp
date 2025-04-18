#pragma once

using namespace std;

#include <map>
#include <string>
#include <glad/glad.h>
#include <Shader/Shader.hpp>

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class Texture {
    public:
        string path;
        GLuint id;
        TextureType type;
        GLuint unit;
        
        Texture(const char* image, TextureType textureType, GLuint slot);

        const char* GetTextureTypeAsString() { return textureTypeToString[type]; }

        void SetTextureUnit(unsigned int shaderID, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();

    private:
        // Textures will be named "diffuse0", "diffuse1", "specular0", "specular1", etc.
        map <TextureType, const char*> textureTypeToString = {
            {TextureType::DIFFUSE, "diffuse"},
            {TextureType::SPECULAR, "specular"}
        };
};