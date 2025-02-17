#pragma once

#include <vector>
#include <json.hpp>
#include <rendering/window/mesh/mesh.hpp>
#include <utilities/utilities.hpp> 

using json = nlohmann::json;

class Model {
    public:
        Model(const char* modelFilePath);

        std::vector<unsigned char> ReadData();
        std::vector<float> GetFloats(json accessor);
        std::vector<GLuint> GetIndices(json accessor);
        std::vector<Texture> GetTextures();
        std::vector<Vertex> AssembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoordinates);
        void TraverseNode(unsigned int nodeIndex, glm::mat4 matrix = glm::mat4(1.0f));
        void Draw(Shader& shader, Camera& camera);

        std::vector<glm::vec2> ToVec2(std::vector<float> floats);
        std::vector<glm::vec3> ToVec3(std::vector<float> floats);
        std::vector<glm::vec4> ToVec4(std::vector<float> floats);

        void LoadMesh(unsigned int meshIndex);
    private:
        const char* modelFilePath;
        std::vector<unsigned char> data;
        json parsedContent;
        std::vector<std::string> loadedTextureNames;
        std::vector<Texture> loadedTextures;
        std::vector<Mesh> meshes;
        std::vector<glm::vec3> translationMeshes;
        std::vector<glm::quat> rotationMeshes;
        std::vector<glm::vec3> scaleMeshes;
        std::vector<glm::mat4> matricesMeshes;
};