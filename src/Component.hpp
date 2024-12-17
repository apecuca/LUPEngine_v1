#pragma once

class GameObject;

class Component
{
public:
	Component(GameObject& parent);
	virtual ~Component() = default;

	// Constructors de cópia
	//Component(const Component& other) {}
	Component(const Component& other) :
		gameObject{ other.gameObject },
		uniqueID{ nextCreationID } {
	}
	Component(Component&& other) = default;

	// Operação de cópia
	Component& operator = (const Component& other) { return *this; }
	Component& operator = (Component&& other) = default;

	// Operação de comparação
	bool operator == (const Component& other) const
	{
		return (other.GetUniqueID() == this->GetUniqueID());
	}

	// Getter do UniqueID (identificação)
	inline int GetUniqueID() const { return uniqueID; }

	bool enabled = true;
	GameObject& gameObject;

	virtual void Update();

protected:
	// Identificador
	int uniqueID;
	static int nextCreationID;
};

class GenericComponent : public Component
{
	GenericComponent(GameObject& parent);
	~GenericComponent() = default;

	void Update() override;
};

class Pointlight : public Component
{
public:
	Pointlight(GameObject& parent);
	~Pointlight();

	float dir = 0.0f;

	void Update() override;
};