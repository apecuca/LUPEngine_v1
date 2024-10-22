#include "GameObject.hpp"

#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"

#include "GLFW/glfw3.h"

int GameObject::nextCreationID = 0;

GameObject::GameObject() :
	uniqueID { nextCreationID }
{
	nextCreationID++;
}

void GameObject::InitShader()
{
	shader = std::make_unique<Shader>(*this);
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	position = glm::vec3(0.0f);
}

void GameObject::UpdateBehaviour()
{
	//Rotate(glm::vec3(0.0f, 25.0f, 0.0f) * Time::deltaTime);
	for (int i = 0; i < components.size(); i++)
	{
		components.at(i)->Update();
		//components.at(i)->Create();
	}
}

void GameObject::Render()
{
	if (shader == nullptr) return;

	shader->Render();
}

void GameObject::Rotate(glm::vec3 angle)
{
	SetRotation(rotation + angle);
}

void GameObject::SetRotation(glm::vec3 angle)
{
	rotation = angle;

	UpdateRotation(angle);
}

void GameObject::SetRotation(glm::vec3 angle, bool clamp)
{
	rotation = angle;

	if (clamp)
		rotation.y = glm::clamp(rotation.y, -89.0f, 89.0f);

	UpdateRotation(angle);
}

void GameObject::UpdateRotation(glm::vec3 lastOperation)
{
	// Gerenciamento
	lastRotation = lastOperation;

	// Axis de rotação
	front = glm::normalize(glm::vec3(
		cos(glm::radians(rotation.y)) * sin(glm::radians(rotation.x)),
		-sin(glm::radians(rotation.y)),
		cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x))
	));

	right = glm::normalize(glm::vec3(
		-cos(glm::radians(rotation.x)),
		0.f,
		sin(glm::radians(rotation.x))
	));

	up = glm::normalize(glm::vec3(
		sin(glm::radians(rotation.y)) * sin(glm::radians(rotation.x)),
		cos(glm::radians(rotation.y)),
		sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x))
	));
}