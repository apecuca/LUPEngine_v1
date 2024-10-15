#pragma once

#include "Debug.hpp"
#include "Shader.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();

	/// Posi��o do objeto em um plano tri-dimensional (mundo)
	glm::vec3 position = glm::vec3(0.0f);
	/// �ngulos de rota��o de um objeto
	glm::vec3 rotation = glm::vec3(0.0f);

	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	///
	void InitShader();

	void Update();
	void Render();

	void SetRotation(glm::vec3 angle);
	void SetRotation(glm::vec3 angle, bool clamp);
	void Rotate(glm::vec3 angle);
private:
	Shader* shader = nullptr;

	void UpdateRotationAxis();
};	