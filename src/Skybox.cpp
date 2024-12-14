#include "Skybox.hpp"

// Standard
#include <fstream>
#include <sstream>

// Load de imagens
#include <stb/stb_image.h>

// Matemática
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Classes LUPEngine
#include "Shader.hpp"
#include "Camera.hpp"
#include "LUPEngine.hpp"

Skybox::Skybox(const std::string rightFile, const std::string leftFile,
    const std::string topFile, const std::string bottomFile,
    const std::string frontFile, const std::string backFile)
{
    // Preparar o 
    cubemapFaces[0] = "Resources/Skybox/" + rightFile;
    cubemapFaces[1] = "Resources/Skybox/" + leftFile;
    cubemapFaces[2] = "Resources/Skybox/" + topFile;
    cubemapFaces[3] = "Resources/Skybox/" + bottomFile;
    cubemapFaces[4] = "Resources/Skybox/" + frontFile;
    cubemapFaces[5] = "Resources/Skybox/" + backFile;

    // Configurar, gerar buffers e texturas
	ConfigSkybox();
	GenerateBufferObjs();
	GenerateTextures();

	// Iniciar matrizes de transformação
	viewMat = glm::mat4(1.0f);
	projecMat = glm::mat4(1.0f);
}

Skybox::~Skybox()
{
    // deletar tudo
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteTextures(1, &cubemapTex);

	glDeleteProgram(ID);
}

void Skybox::Render()
{
    // Depht test passa quando os valores são iguais
    // ao conteúdo do buffer de profundidade
    glDepthFunc(GL_LEQUAL); 
    
    // Bindar programa
    glUseProgram(ID);

    // Matriz de projeção
    projecMat = glm::perspective(glm::radians(Camera::GetInstance().fov),
        (float)(LUPEngine::WIDTH) / (float)(LUPEngine::HEIGHT), 0.1f, 100.0f);
    // Matriz de visualização
    //viewMat = Camera::GetInstance().GetViewMatrix();
    viewMat = glm::mat4(glm::mat3(Camera::GetInstance().GetViewMatrix()));

    // Atualizar matrizes
    SetMat4("view", viewMat);
    SetMat4("projection", projecMat);

    // Draw
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Voltar função de profundidade pro default
    glDepthFunc(GL_LESS);

}

void Skybox::ConfigSkybox()
{
    // Isso aqui tá dentro da função pq provavelmente
    // não vou criar outro shader pra skybox
    const char* vertexPath = "Shaders/skybox.vert";
    const char* fragmentPath = "Shaders/skybox.frag";

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    Shader::CompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    Shader::CompileErrors(fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    Shader::CompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Skybox::GenerateBufferObjs()
{
    // O nome tá óbvio
    float skyboxVertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Gerar e bindar VAO
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Gerar VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Linkar VBO com coordenadas das vértices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::GenerateTextures()
{
    // Gerar e bindar textura
    glGenTextures(1, &cubemapTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);

    // Parâmetros de cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, nrChannels;
    for (int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // Gerar textura se tiver data
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 
                width, height, 0,
                GL_RGB, GL_UNSIGNED_BYTE,
                data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << cubemapFaces[i] << std::endl;
        }

        stbi_image_free(data);
    }

    // Indicar em qual camada do programa está a textura
    glUseProgram(ID);
    SetInt("skybox", 0);
}

void Skybox::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Skybox::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}