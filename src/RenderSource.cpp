#include "RenderSource.hpp"

// Standard
#include <fstream>
#include <sstream>

// Math
#include <glm/gtc/matrix_transform.hpp>

// LUPEngine classes
#include "LUPEngine.hpp"
#include "Debug.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"

RenderSource::RenderSource(GameObject& parent) :
	Component(parent)
{
	LUPEngine::AddRenderSource(*this);
}

RenderSource::~RenderSource()
{
	LUPEngine::RemoveRenderSource(*this);

    glDeleteShader(shaderID);
}

void RenderSource::ConfigShader(const char* vertexFile, const char* fragmentFile, const char* modelFile)
{
    // Path translation
    std::string vertexPath = "Shaders/" + static_cast<std::string>(vertexFile);
    std::string fragmentPath = "Shaders/" + static_cast<std::string>(fragmentFile);

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
    }const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // shader Program
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);
    CheckCompileErrors(shaderID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Aplicar transforms iniciais
    Use();
    SetMat4("view", viewMat);
    SetMat4("projection", projMat);
    SetMat4("model", modelMat);

    // Directional light
    SetVec3("dirLight.direction", Lighting::directional);
    SetVec3("dirLight.ambient", Lighting::ambient);
    SetVec3("dirLight.diffuse", Lighting::diffuse);
    SetVec3("dirLight.specular", Lighting::specular);

    SetFloat("dirLight.ambientStrength", Lighting::ambientStrength);
    SetFloat("dirLight.directionalStrength", Lighting::dirStrength);

    // Point light
    for (int i = 0; i < 10; i++)
    {
        std::string current = "pointLights[" + std::to_string(i) + "].";

        SetFloat(current + "strength", 0.0f);

        SetFloat(current + "constant", 1.0f);
        SetFloat(current + "linear", 0.22f);
        SetFloat(current + "quadratic", 0.20f);

        SetVec3(current + "ambient", Lighting::ambient);
        SetFloat(current + "ambientStrength", Lighting::ambientStrength);
        SetVec3(current + "diffuse", Lighting::diffuse);
        SetVec3(current + "specular", Lighting::specular);
    }

    // Generate model
    std::string modelPath = "Resources/Models/" + static_cast<std::string>(modelFile);
    model = std::make_shared<Model>(modelPath, false);

    /*
    Debug::Log("Meshes count: " + std::to_string(model->meshes.size()));
    for (int i = 0; i < model->meshes.size(); i++)
    {
        std::string vertexCount = "Mesh ";
        vertexCount += std::to_string(i);
        vertexCount += " vertices: ";
        vertexCount += std::to_string(model->meshes.at(i).vertices.size());
        Debug::Log(vertexCount);
    }
    */
}

void RenderSource::Update()
{
	//
}

void RenderSource::Draw()
{
    if (model == nullptr) return;

    Camera& cam = Camera::GetInstance();

    Use();

    // Matriz de projecao
    projMat = glm::perspective(glm::radians(cam.fov),
        (float)LUPEngine::WIDTH / (float)LUPEngine::HEIGHT,
        0.1f, 100.0f);
    viewMat = cam.GetViewMatrix();
    SetMat4("projection", projMat);

    // Matriz de visualização
    viewMat = Camera::GetInstance().GetViewMatrix();
    SetMat4("view", viewMat);

    // Matriz de modelo
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, gameObject.position);
    if (glm::length(gameObject.rotation) != 0)
    {
        modelMat = glm::rotate(modelMat,
            glm::radians(glm::length(gameObject.rotation)),
            glm::normalize(GetCorrectedRotation()));
    }
    modelMat = glm::scale(modelMat, gameObject.scale);
    SetMat4("model", modelMat);

    // Posição da câmera
    SetVec3("viewPos", Camera::GetInstance().position);

    // Point light
    std::vector<glm::vec3> lightSources = Lighting::GetLightSources();
    for (int i = 0; i < 10; i++)
    {
        std::string current = "pointLights[" + std::to_string(i) + "].";

        if (i < lightSources.size())
        {
            SetVec3(current + "position", Lighting::GetLightSources().at(i));
            SetFloat(current + "strength", 1.0f);
        }
        else
        {
            SetFloat(current + "strength", 0.0f);
        }
    }

    // Chamar o draw do modelo
    model->Draw(shaderID);
}

//
// Shader Utilities
//

// activate the shader
void RenderSource::Use() const
{
    glUseProgram(shaderID);
}

// Retorna as rotacoes corrigidas
glm::vec3 RenderSource::GetCorrectedRotation()
{
    return glm::vec3(
        -gameObject.rotation.x,
        -gameObject.rotation.y,
        gameObject.rotation.z);
}

// utility uniform functions
// ------------------------------------------------------------------------
void RenderSource::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void RenderSource::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void RenderSource::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void RenderSource::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void RenderSource::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void RenderSource::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void RenderSource::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void RenderSource::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}
void RenderSource::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void RenderSource::SetMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void RenderSource::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void RenderSource::RenderSource::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// utility function for checking shader compilation/linking errors.
void RenderSource::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}