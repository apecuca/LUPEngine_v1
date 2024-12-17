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

private:
	static std::vector<std::reference_wrapper<Pointlight>> lightSources;
};