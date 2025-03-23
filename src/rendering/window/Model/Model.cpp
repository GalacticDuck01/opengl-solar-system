#include "Model.hpp"

#include "../Mesh/Mesh.hpp"
#include <Camera/Camera.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Draw(Shader& shader, Camera& camera) {
    for (unsigned int im = 0; im < meshes.size(); im++) {
        meshes[im].Draw(shader, camera);
    }
}

void Model::loadModel(string filepath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs); // See http://assimp.sourceforge.net/lib_html/postprocess_8h.html for a list of available post processing flags

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    directory = filepath.substr(0, filepath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(toMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::toMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int iv = 0; iv < mesh->mNumVertices; iv++) {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[iv].x;
        vector.y = mesh->mVertices[iv].y;
        vector.z = mesh->mVertices[iv].z;
        vertex.position = vector;
        // normals
        vector.x = mesh->mNormals[iv].x;
        vector.y = mesh->mNormals[iv].y;
        vector.z = mesh->mNormals[iv].z;
        vertex.normal = vector;

        vertex.colour = glm::vec3(1.0f, 1.0f, 1.0f);

        // texture coordinates
        if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][iv].x; 
            vec.y = mesh->mTextureCoords[0][iv].y;

            vertex.textureCoords = vec;
        }
        else {
            std::cout << "Mesh does not contain texture coordinates" << std::endl;
            vertex.textureCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int iface = 0; iface < mesh->mNumFaces; iface++) {
        aiFace face = mesh->mFaces[iface];
        for (unsigned int ii = 0; ii < face.mNumIndices; ii++) {
            indices.push_back(face.mIndices[ii]);
        }
    }

    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, TextureType type) {
    vector<Texture> textures;

    aiTextureType assimpType = type == TextureType::DIFFUSE  ? aiTextureType_DIFFUSE : aiTextureType_SPECULAR;

    for (unsigned int it = 0; it < mat->GetTextureCount(assimpType); it++) {
        aiString str;
        mat->GetTexture(assimpType, it, &str);

        string textureFullPath = directory + '/' + str.C_Str();

        bool skip = false;
        for (unsigned int il = 0; il < loadedTextures.size(); il++) {
            if (strcmp(loadedTextures[il].path.c_str(),textureFullPath.c_str()) == 0) {
                textures.push_back(loadedTextures[il]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) { // if texture hasn't been loaded already, load it
            Texture texture(textureFullPath.c_str(), type, it);
            textures.push_back(texture);
            loadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }

    return textures;
}