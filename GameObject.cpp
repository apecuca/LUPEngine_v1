#include "GameObject.hpp"

#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"

#include "GLFW/glfw3.h"

void GameObject::InitShader(glm::vec3 color)
{
	//shader = new Shader(color);
	shader = std::make_unique<Shader>(*this, color);
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
}

void GameObject::Update()
{
	Rotate(glm::vec3(0.0f, 45.0f, 0.0f) * Time::deltaTime);
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

void GameObject::SetPosition(glm::vec3 pos)
{
	position = pos;
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

	// Shader
	/*
	if (shader == nullptr) return;
	shader->Rotate(glm::vec3(
		lastOperation.y,
		lastOperation.x,
		lastOperation.z
	));
	*/
}