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
    GLuint id = 0;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    // Default constructors
    Mesh(std::vector<Vertex> _vertices,
        std::vector<GLuint> _indices,
        std::vector<Texture> _textures);
    ~Mesh();

    // Constructors de cópia
    Mesh(const Mesh& other) :
        Mesh(other.vertices, other.indices, other.textures) {};
    Mesh(Mesh&& other) = default;

    // Operação de cópia
    Mesh& operator = (const Mesh& other) { return *this; }
    Mesh& operator = (Mesh&& other) = default;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLuint VAO;

    void SetupTextureUniforms(GLuint shaderID);

    void Draw(GLuint shader);

private:
    GLuint VBO, EBO;

    void SetupMesh();
};