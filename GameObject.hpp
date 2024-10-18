#pragma once

#include "Shader.hpp"

class GameObject
{
public:
	// Constructors padrão
	GameObject() = default;
	~GameObject() = default;

	// Constructors de cópia
	GameObject(const GameObject& other) {}
	GameObject(GameObject&& other) = default;

	// Operação de cópia
	GameObject& operator=(const GameObject& other) { return *this; }
	GameObject& operator=(GameObject&& other) = default;

	/// Posição do objeto em um plano tri-dimensional (mundo)
	glm::vec3 position = glm::vec3(0.0f);
	/// Ângulos de rotação de um objeto
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 lastRotation = glm::vec3(0.f);

	// Axis de orientação
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	//
	void InitShader();

	void Update();
	void Render();

	// Rotação
	void SetRotation(glm::vec3 angle);
	void SetRotation(glm::vec3 angle, bool clamp);
	void Rotate(glm::vec3 angle);

	// Posição
	void SetPosition(glm::vec3 pos);
private:
	// Renderização
	std::unique_ptr<Shader> shader;

	void UpdateRotation(glm::vec3 lastOperation);
};	