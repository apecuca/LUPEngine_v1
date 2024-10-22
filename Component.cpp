#include "Component.hpp"

#include "glm/glm.hpp"

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Time.hpp"

Component::Component(GameObject& parent) :
	gameObject {parent}
{
	// Base constructor
}

void Component::Update() {}

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