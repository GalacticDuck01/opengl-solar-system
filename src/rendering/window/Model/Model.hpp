#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "../Texture/Texture.hpp"

using namespace std;

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;
class Mesh;
class Camera;
class Shader;

class Model {
private:
    vector<Mesh> meshes;
    vector<Texture> loadedTextures;
    string directory;

    void loadModel(string filepath);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh toMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, TextureType type);
public:
    Model(const char* filepath) {loadModel(filepath); };
    void Draw(Shader& shader, Camera& camera);
};