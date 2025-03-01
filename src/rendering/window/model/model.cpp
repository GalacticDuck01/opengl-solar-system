#include <Rendering/Window/Model/Model.hpp>

Model::Model(const char* modelFilePath) {
    std::string contents = ReadFile(modelFilePath);
    this->modelFilePath = modelFilePath;
    this->parsedContent = json::parse(contents);
    this->data = ReadData();

    TraverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader, camera, matricesMeshes[i]);
    }
}

std::vector<unsigned char> Model::ReadData() {
    std::string bytesText;
    std::string uri = parsedContent["buffers"][0]["uri"];

    std::string file = std::string(modelFilePath);
    std::string fileDirectory = file.substr(0, file.find_last_of("/") + 1);
    bytesText = ReadFile(fileDirectory + uri);

    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

std::vector<float> Model::GetFloats(json accessor) {
    std::vector<float> floats;
    unsigned int bufferViewIndex = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = parsedContent["bufferViews"][bufferViewIndex];
    unsigned int byteOffset = bufferView.value("byteOffset", 0);

    unsigned int numPerVect;
    if (type == "SCALAR") {
        numPerVect = 1;
    } else if (type == "VEC2") {
        numPerVect = 2;
    } else if (type == "VEC3") {
        numPerVect = 3;
    } else if (type == "VEC4") {
        numPerVect = 4;
    } else {
        throw std::invalid_argument("Unsupported accessor type (requires SCALAR, VEC2, VEC3, or VEC4)");
    }

    unsigned int beginningOfData = byteOffset + accessorByteOffset;
    unsigned int lengthOfData = count*numPerVect*sizeof(float);
    for (unsigned int i = 0; i < lengthOfData; i += sizeof(float)) {
        unsigned int index = beginningOfData + i;
        float value = *(float*)&data[index];
        floats.push_back(value);
    }
    
    return floats;
}

std::vector<GLuint> Model::GetIndices(json accessor) {
    std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = parsedContent["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i += 4)
		{
			unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
		{
			unsigned char bytes[] = { data[i], data[i + 1] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2)
		{
			unsigned char bytes[] = { data[i], data[i + 1] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::GetTextures() {
    std::vector<Texture> textures;
    std::string fileStr = std::string(modelFilePath);
    std::string fileDir = fileStr.substr(0, fileStr.find_last_of("/") + 1);

    unsigned int unit = 0;
    for (unsigned int i = 0; i < parsedContent["images"].size(); i++) {
        std::string texturePath = parsedContent["images"][i]["uri"];

        bool skip = false;
        for (unsigned int j = 0; j < loadedTextureNames.size(); j++) {
            if (loadedTextureNames[j] == texturePath) {
                loadedTextures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            if (texturePath.find("baseColor") != std::string::npos) {
                Texture texture((fileDir + texturePath).c_str(), TextureType::DIFFUSE, loadedTextures.size());
                textures.push_back(texture);
                loadedTextures.push_back(texture);
                loadedTextureNames.push_back(texturePath);
            }
            else if (texturePath.find("metallicRoughness") != std::string::npos) {
                Texture texture((fileDir + texturePath).c_str(), TextureType::SPECULAR, loadedTextures.size());
                textures.push_back(texture);
                loadedTextures.push_back(texture);
                loadedTextureNames.push_back(texturePath);
            }
        }
    }

    return textures;
}

std::vector<glm::vec2> Model::ToVec2(std::vector<float> floats) {
    std::vector<glm::vec2> vec2s;
    for (unsigned int i = 0; i < floats.size(); i += 2) {
        vec2s.push_back(glm::vec2(floats[i], floats[i + 1]));
    }
    return vec2s;
}

std::vector<glm::vec3> Model::ToVec3(std::vector<float> floats) {
    std::vector<glm::vec3> vec3s;
    for (unsigned int i = 0; i < floats.size(); i += 3) {
        vec3s.push_back(glm::vec3(floats[i], floats[i + 1], floats[i + 2]));
    }
    return vec3s;
}

std::vector<glm::vec4> Model::ToVec4(std::vector<float> floats) {
    std::vector<glm::vec4> vec4s;
    for (unsigned int i = 0; i < floats.size(); i += 4) {
        vec4s.push_back(glm::vec4(floats[i], floats[i + 1], floats[i + 2], floats[i + 3]));
    }
    return vec4s;
}

std::vector<Vertex> Model::AssembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoordinates) {
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < positions.size(); i++) {
        vertices.push_back(Vertex{positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), textureCoordinates[i]});
    }
    return vertices;
}

void Model::LoadMesh(unsigned int meshIndex) {
    unsigned int positionAccessorIndex = parsedContent["meshes"][meshIndex]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccessorIndex = parsedContent["meshes"][meshIndex]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int textureCoordinateAccessorIndex = parsedContent["meshes"][meshIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indicesAccessorIndex = parsedContent["meshes"][meshIndex]["primitives"][0]["indices"];

    std::vector<float> positionFloats = GetFloats(parsedContent["accessors"][positionAccessorIndex]);
    std::vector<glm::vec3> positions = ToVec3(positionFloats);
    std::vector<float> normalFloats = GetFloats(parsedContent["accessors"][normalAccessorIndex]);
    std::vector<glm::vec3> normals = ToVec3(normalFloats);
    std::vector<float> textureCoordinateFloats = GetFloats(parsedContent["accessors"][textureCoordinateAccessorIndex]);
    std::vector<glm::vec2> textureUVs = ToVec2(textureCoordinateFloats);
    
    std::vector<Vertex> vertices = AssembleVertices(positions, normals, textureUVs);
    std::vector<unsigned int> indices = GetIndices(parsedContent["accessors"][indicesAccessorIndex]);
    std::vector<Texture> textures = GetTextures();

    meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::TraverseNode(unsigned int nodeIndex, glm::mat4 matrix) {
    json node = parsedContent["nodes"][nodeIndex];
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end()) {
        translation = glm::vec3(node["translation"][0], node["translation"][1], node["translation"][2]);
    }

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end()) {
        rotation = glm::quat(node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]);
    }

    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end()) {
        scale = glm::vec3(node["scale"][0], node["scale"][1], node["scale"][2]);
    }

    glm::mat4 transform = glm::mat4(1.0f);
    if (node.find("matrix") != node.end()) {
        float matrixValues[16];
        for (unsigned int i = 0; i < 16; i++) {
            matrixValues[i] = node["matrix"][i];
        }
        transform = glm::make_mat4(matrixValues);
    }

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    glm::mat4 model = matrix * transform * trans * rot * sca;

    if (node.find("mesh") != node.end()) {
        translationMeshes.push_back(translation);
        rotationMeshes.push_back(rotation);
        scaleMeshes.push_back(scale);
        matricesMeshes.push_back(model);

        LoadMesh(node["mesh"]);
    }

    for (unsigned int i = 0; i < node["children"].size(); i++) {
        TraverseNode(node["children"][i], model);
    }
}