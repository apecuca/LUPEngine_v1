#include "Component.hpp"

#include "glm/glm.hpp"

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "LUPEngine.hpp"

Component::Component(GameObject& parent) :
	gameObject {parent}
{
	// Base constructor
}

void Component::Update() {}

//
// Generic component
//

GenericComponent::GenericComponent(GameObject& parent) :
	Component(parent)
{
	// Derived constructor example
}

void GenericComponent::Update()
{
	// Behaviour example
	// gameObject.rotation += glm::vec3(1.0f) * 45.0f * Time::deltaTime;
}

//
// Light cube
//

Pointlight::Pointlight(GameObject& parent) :
	Component(parent)
{
	// Derived constructor example
}

void Pointlight::Update()
{
	// Behaviour example

	float moveSpeed = 3.25f * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_RIGHT)) gameObject.position += glm::vec3(moveSpeed, 0, 0);
	if (Input::GetKey(GLFW_KEY_LEFT)) gameObject.position += glm::vec3(-moveSpeed, 0, 0);
	if (Input::GetKey(GLFW_KEY_UP)) gameObject.position += glm::vec3(0, 0, -moveSpeed);
	if (Input::GetKey(GLFW_KEY_DOWN)) gameObject.position += glm::vec3(0, 0, moveSpeed);

	LUPEngine::lightSource = gameObject.position;
}