#include "GameObject.hpp"

#include "Time.hpp"
#include "Input.hpp"

#include "GLFW/glfw3.h"

GameObject::GameObject()
{
	SetRotation(glm::vec3(0.f));
}

GameObject::~GameObject()
{
	delete shader;
}

void GameObject::InitShader()
{
	shader = new Shader();
}

void GameObject::Update()
{
	//
}

void GameObject::Render()
{
	if (shader == nullptr) return;

	shader->Render();
}

void GameObject::SetRotation(glm::vec3 angle)
{
	rotation = angle;

	UpdateRotationAxis();
}

void GameObject::SetRotation(glm::vec3 angle, bool clamp)
{
	rotation = angle;

	if (clamp)
		rotation.y = glm::clamp(rotation.y, -89.0f, 89.0f);

	UpdateRotationAxis();
}

void GameObject::Rotate(glm::vec3 angle)
{
	rotation += angle;

	UpdateRotationAxis();
}

void GameObject::UpdateRotationAxis()
{
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