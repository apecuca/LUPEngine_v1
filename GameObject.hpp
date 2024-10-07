#pragma once

#include "Debug.hpp"
#include "Shader.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	//
	void Update();
	void Render();

private:
	Shader shader;
};	