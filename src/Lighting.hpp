#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Component.hpp"

class Lighting
{
public:
	static void AddLightSource(Pointlight& source);
	static void RemoveLightSource(Pointlight& source);
	static std::vector<glm::vec3> GetLightSources();

	// Light data
	static glm::vec3 directional;
	static glm::vec3 ambient;
	static glm::vec3 diffuse;
	static glm::vec3 specular;
	static float ambientStrength;
	static float dirStrength;

private:
	static std::vector<std::reference_wrapper<Pointlight>> lightSources;
};