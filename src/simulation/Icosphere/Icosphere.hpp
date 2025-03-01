#pragma once

#include <Rendering/Window/Mesh/Mesh.hpp>
#include <Shader/Shader.hpp>
#include <Camera/Camera.hpp>

#include <glm/glm.hpp>

using namespace glm;

class Icosphere {
private:
    struct TriIndex {
        int index0;
        int index1;
        int index2;

        TriIndex(int index0, int index1, int index2) {
            this->index0 = index0;
            this->index1 = index1;
            this->index2 = index2;
        }
    };
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<TriIndex> triangles;
    

    void GenerateIcosphere();
    void UpdateMesh();
    int CreateNewMidpoint(int index1, int index2);

public:
    glm::vec3 position;
    float radius;
    int resolution;
    Mesh mesh;

    Icosphere(vec3 position, float radius, int resolution);
    ~Icosphere() {};

    void Draw(Shader& shader, Camera& camera) {mesh.Draw(shader, camera);};

};