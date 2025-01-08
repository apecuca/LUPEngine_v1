#pragma once

// Standard
#include <iostream>
#include <vector>
#include <string>

// Math and OpenGL
#include "glad/glad.h"
#include "glm/glm.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> _vertices,
        std::vector<GLuint> _indices,
        std::vector<Texture> _textures);
    ~Mesh() = default;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLuint VAO;

    void Draw(GLuint shader);

private:
    GLuint VBO, EBO;

    void SetupMesh();
};