#pragma once

// Standard classes
#include <vector>
#include <iostream>

// LUPE classes
#include "Shader.hpp"
#include "Component.hpp"
#include "Debug.hpp"

class GameObject
{
public:
	// Constructors padrão
	GameObject();
	virtual ~GameObject();

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

	void InitShader(const int diffuseMapIndex = 0,
		const int specularMapIndex = 3,
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag");

	void UpdateBehaviour();
	void Render();

	// Rotação
	void SetRotation(glm::vec3 angle);
	void SetRotation(glm::vec3 angle, bool clamp);
	void Rotate(glm::vec3 angle);

	// Adiciona um componente ao gameobject e retorna um pointer do componente adicionado
	template <class T> std::shared_ptr<T> AddComponent()
	{
		if (components.size() - 1 >= components.capacity())
		{
			components.reserve(4);
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(*this);
		components.push_back(newComponent);

		return newComponent;
	}

	// Retorna um pointer do primeiro componente do tipo 
	template <class T> auto GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (typeid(T).name() != typeid(*components.at(i).get()).name())
				continue;

			return components.at(i);
		}

		throw std::invalid_argument("No available component");
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
	std::vector<std::shared_ptr<Component>> components;

	void UpdateRotation(glm::vec3 lastOperation);
};	