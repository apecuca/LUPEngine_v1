#include "Model.hpp"

#include "Debug.hpp"
#include "stb_image.h"

Model::Model(std::string const& path, GLuint& shaderID, bool gamma) :
	gammaCorrection {gamma},
    shader {shaderID}
{
	LoadModel(path);
}

void Model::Draw()
{
    for (int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

// loads a model with supported ASSIMP extensions from file 
// and stores the resulting meshes in the meshes vector.
void Model::LoadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | 
        aiProcess_CalcTangentSpace);
    
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located
// at the node and repeats this process on its children nodes (if any).
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Alocar espaço necessário para evitar cópias
    int emptySpace = meshes.capacity() - meshes.size();
    int allocNeeded = node->mNumMeshes + node->mNumChildren;
    if (allocNeeded > emptySpace)
    {
        int nextAlloc = allocNeeded - emptySpace;
        meshes.reserve(nextAlloc);
    }

    // process each mesh located at the current node
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        //aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //meshes.push_back(ProcessMesh(mesh, scene));

        // Processar informação da Mesh e copiar para uma nova instância
        MeshData newMeshData = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
        meshes.emplace_back(newMeshData.vertices, newMeshData.indices, newMeshData.textures);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }

    // Fazer setup das texturas das meshes
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].SetupTextureUniforms(shader);
    }
}

Model::MeshData Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    //std::vector<Vertex> vertices;
    //std::vector<GLuint> indices;
    //std::vector<Texture> textures;
    MeshData newMesh;

    // walk through each of the mesh's vertices
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        newMesh.vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (int j = 0; j < face.mNumIndices; j++)
            newMesh.indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    /*
    aiString texFile_diffuse;
    material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texFile_diffuse);
    bool embedded = false;
    if ('*' == texFile_diffuse.data[0])
        embedded = true;
        */

    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        newMesh.textures.insert(newMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 2. specular maps
        std::vector<Texture> specularMaps = LoadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular", scene);
        newMesh.textures.insert(newMesh.textures.end(), specularMaps.begin(), specularMaps.end());

        // 3. normal maps
        std::vector<Texture> normalMaps = LoadMaterialTextures(
            material, aiTextureType_HEIGHT, "texture_normal", scene);
        newMesh.textures.insert(newMesh.textures.end(), normalMaps.begin(), normalMaps.end());

        // 4. height maps
        std::vector<Texture> heightMaps = LoadMaterialTextures(
            material, aiTextureType_AMBIENT, "texture_height", scene);
        newMesh.textures.insert(newMesh.textures.end(), heightMaps.begin(), heightMaps.end());
    }
    else
    {
        // Load default diffuse texture
        Texture defaultTex;
        defaultTex.id = TextureFromFile("default_tex.png", "Resources/Images");
        defaultTex.type = "texture_diffuse";
        defaultTex.path = "default_tex.png";
        newMesh.textures.push_back(defaultTex);
    }
    

    // return a mesh object created from the extracted mesh data
    return newMesh;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> Model::LoadMaterialTextures(
    aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            
            // Embedded texture warning
            auto embeddedTex = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTex)
            {
                texture.id = EmbeddedTexture(embeddedTex->pcData, embeddedTex->mWidth);
            }
            else
                texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }

    return textures;
}

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
		GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else Debug::Log("Unsupported format type");

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
    }

    // Unbind textura
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free space
    stbi_image_free(data);

    return textureID;
}

GLuint EmbeddedTexture(void* pData, int bufferSize)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture image and generate mipmaps
    int width, height, channels;
    unsigned char* data = stbi_load_from_memory((const stbi_uc*)pData, bufferSize, &width, &height, &channels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else Debug::Log("Unsupported format type");

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture from memory." << std::endl;
        return 0; // Return 0 to indicate failure
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free image data
    stbi_image_free(data);

    return textureID;

}