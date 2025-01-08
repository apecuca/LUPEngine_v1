#include "Component.hpp"

#include "glm/glm.hpp"

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "LUPEngine.hpp"
#include "Lighting.hpp"

int Component::nextCreationID = 0;

Component::Component(GameObject& parent) :
	gameObject {parent},
	uniqueID {nextCreationID},
	enabled {true}
{
	// UniqueID handling
	nextCreationID++;
}

void Component::Update() {}

//
// Generic component
// for testing :)
//

GenericComponent::GenericComponent(GameObject& parent) :
	Component(parent)
{
	// Constructor example
}

void GenericComponent::Update()
{
	// Custom behaviour example
}

//
// Point light
//

Pointlight::Pointlight(GameObject& parent) :
	Component(parent)
{
	// Derived constructor example
	
	Lighting::AddLightSource(*this);
}

Pointlight::~Pointlight()
{
	Lighting::RemoveLightSource(*this);
}

void Pointlight::Update()
{
	// Behaviour example

	if (Input::GetKey(GLFW_KEY_DOWN))
		floatDist -= 0.75f * Time::deltaTime;
	if (Input::GetKey(GLFW_KEY_UP))
		floatDist += 0.75f * Time::deltaTime;

	gameObject.position = glm::vec3(
		sin(glfwGetTime()) * floatDist * dir,
		sin(glfwGetTime()) * floatDist * dir,
		cos(glfwGetTime()) * floatDist * dir);

	/*
	float moveSpeed = 3.25f * Time::deltaTime;

	gameObject.position = glm::vec3(
		sin(glfwGetTime()) * 2.0f * dir,
		sin(glfwGetTime()) * 2.0f * dir,
		cos(glfwGetTime()) * 2.0f * dir);

	if (Input::GetKey(GLFW_KEY_RIGHT)) gameObject.position += glm::vec3(moveSpeed, 0, 0);
	if (Input::GetKey(GLFW_KEY_LEFT)) gameObject.position += glm::vec3(-moveSpeed, 0, 0);
	if (Input::GetKey(GLFW_KEY_UP)) gameObject.position += glm::vec3(0, 0, -moveSpeed);
	if (Input::GetKey(GLFW_KEY_DOWN)) gameObject.position += glm::vec3(0, 0, moveSpeed);
	*/

	//LUPEngine::lightSource = gameObject.position;
}