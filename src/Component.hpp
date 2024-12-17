#pragma once

class GameObject;

class Component
{
public:
	Component(GameObject& parent);
	~Component() = default;

	// Constructors de c�pia
	//Component(const Component& other) {}
	Component(const Component& other) :
		gameObject{ other.gameObject } {}
	Component(Component&& other) = default;

	// Opera��o de c�pia
	Component& operator = (const Component& other) { return *this; }
	Component& operator = (Component&& other) = default;

	bool enabled = true;
	GameObject& gameObject;

	virtual void Update();
};

class GenericComponent : public Component
{
public:
	GenericComponent(GameObject& parent);
	void Update() override;
};

class Pointlight : public Component
{
public:
	Pointlight(GameObject& parent);
	void Update() override;
};