#pragma once

// Standard classes
#include <vector>
#include <iostream>

// LUPE classes
#include "Shader.hpp"
#include "Component.hpp"

class GameObject
{
public:
	// Constructors padrão
	GameObject();
	~GameObject() = default;

	// Constructors de cópia
	GameObject(const GameObject& other) : 
		uniqueID {nextCreationID} {}
	GameObject(GameObject&& other) = default;

	// Operação de cópia
	GameObject& operator = (const GameObject& other) { return *this; }
	GameObject& operator = (GameObject&& other) = default;

	// Operação de comparação
	bool operator == (const GameObject& other) const
	{
		return (other.GetUniqueID() == this->GetUniqueID());
	}

	// Getter do UniqueID (identificação)
	inline int GetUniqueID() const { return uniqueID; }

	/// Posição do objeto em um plano tri-dimensional (mundo)
	glm::vec3 position = glm::vec3(0.0f);
	/// Ângulos de rotação de um objeto
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	// Axis de orientação
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	//
	void InitShader();

	void UpdateBehaviour();
	void Render();

	// Rotação
	void SetRotation(glm::vec3 angle);
	void SetRotation(glm::vec3 angle, bool clamp);
	void Rotate(glm::vec3 angle);

	// Adiciona uma classe derivada de Component como um componente do objeto
	template <class T> void AddComponent()
	{
		components.reserve(1);
		components.push_back(std::make_unique<T>(*this));
	}

private:
	// Identificador
	int uniqueID;
	static int nextCreationID;

	//
	glm::vec3 lastRotation = glm::vec3(0.f);

	// Renderização
	std::unique_ptr<Shader> shader;

	// Components
	std::vector<std::unique_ptr<Component>> components;

	void UpdateRotation(glm::vec3 lastOperation);
};	