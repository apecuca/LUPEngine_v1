#include "Lighting.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"

// Static variable initializations
std::vector<std::reference_wrapper<Pointlight>> Lighting::lightSources;

glm::vec3 Lighting::directional = glm::vec3(-1.0f, -0.5f, -1.0);
glm::vec3 Lighting::ambient = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
glm::vec3 Lighting::diffuse = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
glm::vec3 Lighting::specular = glm::vec3(1.0f, 0.9568627f, 0.8392157f);

float Lighting::ambientStrength = 0.35f;
float Lighting::dirStrength = 0.8f;

void Lighting::AddLightSource(Pointlight& source)
{
	lightSources.emplace_back(source);
}

void Lighting::RemoveLightSource(Pointlight& source)
{
	for (int i = 0; i < lightSources.size(); i++)
	{
		if (lightSources.at(i) != source)
			continue;

		lightSources.erase(lightSources.begin() + i);
	}
}

std::vector<glm::vec3> Lighting::GetLightSources()
{
	std::vector<glm::vec3> sources;
	for (int i = 0; i < lightSources.size(); i++)
		sources.emplace_back(lightSources.at(i).get().gameObject.position);

	return sources;
}

/*
std::vector<Pointlight&> Lighting::lightSources;

void Lighting::AddLightSource(Pointlight& source)
{
	//lightSources.emplace_back(source);
}

void Lighting::RemoveLightSource(Pointlight& source)
{
	for (int i = 0; i < lightSources.size(); i++)
	{
		if (lightSources.at(i) != source)
			continue;

		lightSources.erase(lightSources.begin() + i);
	}
}

std::vector<glm::vec3> Lighting::GetLightSources()
{
	std::vector<glm::vec3> sources;
	for (int i = 0; i < lightSources.size(); i++)
		sources.emplace_back(lightSources.at(i).gameObject.position);

	return sources;
}
*/