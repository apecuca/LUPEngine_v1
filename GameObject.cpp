#include "GameObject.hpp"

#include "Time.hpp"
#include "Input.hpp"

#include "GLFW/glfw3.h"

GameObject::GameObject()
{
	//
}

GameObject::~GameObject()
{
	//
}

void GameObject::Update()
{
	float moveSpeed = 3.0f * Time::deltaTime;

	position += right * (moveSpeed * Input::horizontal);
	position += front * (moveSpeed * Input::vertical);

	if (Input::GetKey(GLFW_KEY_SPACE))
		position += up * moveSpeed;
	if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
		position -= up * moveSpeed;

	if (Input::GetKey(GLFW_KEY_R))
		position = glm::vec3(0.0f);
}

void GameObject::Render()
{
	shader.Render(position, front, up);
}