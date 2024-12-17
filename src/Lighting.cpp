#include "Lighting.hpp"
#include "GameObject.hpp"
#include "Debug.hpp"

std::vector<std::reference_wrapper<Pointlight>> Lighting::lightSources;

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