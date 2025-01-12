#include "RenderSource.hpp"

// Math
#include <glm/gtc/matrix_transform.hpp>

// LUPEngine classes
#include "LUPEngine.hpp"
#include "Debug.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"
#include "Rendering.hpp"

RenderSource::RenderSource(GameObject& parent) :
	Component(parent),
    color {glm::vec4(1.0f)},
    shininess {32.0f},
    projMat {glm::mat4(1.0f)},
    viewMat {glm::mat4(1.0f)},
    modelMat {glm::mat4(1.0f)}
{
	//LUPEngine::AddRenderSource(*this);
    Rendering::AddRenderSource(*this);
}

RenderSource::~RenderSource()
{
	//LUPEngine::RemoveRenderSource(*this);
    Rendering::RemoveRenderSource(*this);
}

void RenderSource::ConfigShader(const char* vertexFile, const char* fragmentFile, const char* modelFile)
{
    // Generate Shader
    shader = std::make_shared<Shader>(vertexFile, fragmentFile);

    // Aplicar transforms iniciais
    shader->Use();
    shader->SetMat4("view", viewMat);
    shader->SetMat4("projection", projMat);
    shader->SetMat4("model", modelMat);

    // Material
    shader->SetBool("material.hasSpecular", false);
    shader->SetVec4("material.color", color);
    shader->SetFloat("material.shininess", shininess);

    // Directional light
    shader->SetVec3("dirLight.direction", Lighting::directional);
    shader->SetVec3("dirLight.ambient", Lighting::lightColor);
    shader->SetVec3("dirLight.diffuse", Lighting::lightColor);
    shader->SetVec3("dirLight.specular", Lighting::lightColor);

    shader->SetFloat("dirLight.ambientStrength", Lighting::ambientStrength);
    shader->SetFloat("dirLight.directionalStrength", Lighting::dirStrength);

    // Point light
    for (int i = 0; i < 10; i++)
    {
        std::string current = "pointLights[" + std::to_string(i) + "].";

        shader->SetFloat(current + "strength", 0.0f);

        shader->SetFloat(current + "constant", 1.0f);
        shader->SetFloat(current + "linear", 0.22f);
        shader->SetFloat(current + "quadratic", 0.20f);

        shader->SetVec3(current + "ambient", Lighting::lightColor);
        shader->SetFloat(current + "ambientStrength", Lighting::ambientStrength);
        shader->SetVec3(current + "diffuse", Lighting::lightColor);
        shader->SetVec3(current + "specular", Lighting::lightColor);
    }

    // Material
    color = glm::vec4(1.0f);
    shininess = 32.0f;

    // Generate model
    std::string modelPath = "Resources/Models/" + static_cast<std::string>(modelFile);
    model = std::make_shared<Model>(modelPath, *shader.get(), false);
}

void RenderSource::Update()
{
    //
}

void RenderSource::Draw()
{
    if (shader == nullptr) return;

    // Draw shader
    Camera& cam = Camera::GetInstance();

    shader->Use();

    // Matriz de projecao
    projMat = glm::perspective(glm::radians(cam.fov),
        (float)LUPEngine::WIDTH / (float)LUPEngine::HEIGHT,
        0.1f, 100.0f);
    viewMat = cam.GetViewMatrix();
    shader->SetMat4("projection", projMat);

    // Matriz de visualização
    viewMat = Camera::GetInstance().GetViewMatrix();
    shader->SetMat4("view", viewMat);

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
    shader->SetMat4("model", modelMat);

    // Posição da câmera
    shader->SetVec3("viewPos", Camera::GetInstance().position);

    // Point light
    std::vector<glm::vec3> lightSources = Lighting::GetLightSources();
    for (int i = 0; i < 10; i++)
    {
        std::string current = "pointLights[" + std::to_string(i) + "].";

        if (i < lightSources.size())
        {
            shader->SetVec3(current + "position", Lighting::GetLightSources().at(i));
            shader->SetFloat(current + "strength", 1.0f);
        }
        else
        {
            shader->SetFloat(current + "strength", 0.0f);
        }
    }

    // Material data
    shader->SetVec4("material.color", color);

    
    if (model != nullptr)
    {
        // Chamar o draw do modelo
        model->Draw();
    }
}

// Retorna as rotacoes corrigidas
glm::vec3 RenderSource::GetCorrectedRotation()
{
    return glm::vec3(
        -gameObject.rotation.x,
        -gameObject.rotation.y,
        gameObject.rotation.z);
}