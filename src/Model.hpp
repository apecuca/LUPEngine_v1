#pragma once

#include "Mesh.hpp"

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    Model(std::string const& path, GLuint& shaderID, bool gamma = false);
    ~Model() = default;

    // Construtor de cópia
    Model(const Model& other) = default;
    Model(Model&& other) = default;

    // Operação de cópia
    Model& operator = (const Model& other) { return *this; }
    Model& operator = (Model&& other) = default;

    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    void Draw();

private:
    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
    };

    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    GLuint& shader;
};