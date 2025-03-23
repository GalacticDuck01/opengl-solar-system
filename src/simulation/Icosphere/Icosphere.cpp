#include <Simulation/Icosphere/Icosphere.hpp>

Icosphere::Icosphere(glm::vec3 position, float radius, int resolution) {
    this->position = position;
    this->radius = radius;
    this->resolution = resolution;

    generateIcosphere();
}

void Icosphere::generateIcosphere() {
    // Create the icosphere, very nicely sourced from http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
    const float t = (1.0f + sqrt(5.0f)) / 2.0f; // Golden ratio
    vertices = {
        vec3(-1,  t,  0),
        vec3( 1,  t,  0),
        vec3(-1, -t,  0),
        vec3( 1, -t,  0),
        vec3( 0, -1,  t),
        vec3( 0,  1,  t),
        vec3( 0, -1, -t),
        vec3( 0,  1, -t),
        vec3( t,  0, -1),
        vec3( t,  0,  1),
        vec3(-t,  0, -1),
        vec3(-t,  0,  1)
    };

    // Normalise vertices
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = glm::normalize(vertices[i]);
    }

    triangles = {
        {0, 11, 5},
        {0,  5, 1},
        {0,  1, 7},
        {0,  7, 10},
        {0, 10, 11},
        {1,  5, 9},
        {5,  11, 4},
        {11, 10, 2},
        {10, 7, 6},
        {7, 1, 8},
        {3, 9, 4},
        {3, 4, 2},
        {3, 2, 6},
        {3, 6, 8},
        {3, 8, 9},
        {4, 9, 5},
        {2, 4, 11},
        {6, 2, 10},
        {8, 6, 7},
        {9, 8, 1}
    };

    int i1, i2, i3, i12, i13, i23;
    for (int i = 0; i < resolution; i++) {
        std::vector<TriIndex> newTriangles;
        for (const auto& tri : triangles) {
            i1 = tri.index0;
            i2 = tri.index1;
            i3 = tri.index2;
            i12 = createNewMidpoint(i1, i2);
            i13 = createNewMidpoint(i1, i3);
            i23 = createNewMidpoint(i2, i3);

            newTriangles.push_back({i1,  i12, i13});
            newTriangles.push_back({i13, i12, i23});
            newTriangles.push_back({i12,  i2, i23});
            newTriangles.push_back({i13, i23,  i3});
        }
        triangles = newTriangles;
    }

    // Calculate normals
    normals.assign(vertices.size(), vec3(0.0f, 0.0f, 0.0f));
    for (const auto& tri : triangles) {
        vec3 v1 = vertices[tri.index0];
        vec3 v2 = vertices[tri.index1];
        vec3 v3 = vertices[tri.index2];
        vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        normals[tri.index0] += normal;
        normals[tri.index1] += normal;
        normals[tri.index2] += normal;
    }

    updateMesh();
}

int Icosphere::createNewMidpoint(int i1, int i2) {
    vec3 v1 = vertices[i1];
    vec3 v2 = vertices[i2];
    vec3 midpoint = (v1 + v2) / 2.0f;
    midpoint = glm::normalize(midpoint);
    // Get index before adding new vertex
    // E.g. if we add the 13th vertex (the first new one), we want to return 12
    int index = vertices.size();
    vertices.push_back(midpoint);
    return index;
}

void Icosphere::updateMesh() {
    std::vector<Vertex> meshVertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int iv = 0; iv < vertices.size(); iv++) {
        Vertex vertex;
        vertex.position = vertices[iv];
        vertex.normal = normals[iv];

        // TODO: Update with actual colour, but for the moment keep as white
        vertex.colour = glm::vec3(1.0f, 0.5f, 0.31f);

        // Icosphere has no texture, just a white colour
        vertex.textureCoords = glm::vec2(0.0f, 0.0f);

        meshVertices.push_back(vertex);
    }

    for (const auto& tri : triangles) {
        indices.push_back(tri.index0);
        indices.push_back(tri.index1);
        indices.push_back(tri.index2);
    }

    Texture texture("resources/textures/blank.png", TextureType::DIFFUSE, 0);
    textures.push_back(texture);

    mesh = Mesh(meshVertices, indices, textures);
}