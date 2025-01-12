#include "Rendering.hpp"

std::vector <std::reference_wrapper<RenderSource>> Rendering::renderSources;

void Rendering::Init()
{


	//glGenFramebuffers(1, &FBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);

}

void Rendering::Render()
{
	// Desenhar fontes de renderização
	for (int i = 0; i < renderSources.size(); i++)
	{
		if (!renderSources.at(i).get().enabled) continue;

		renderSources.at(i).get().Draw();
	}
}

void Rendering::AddRenderSource(RenderSource& source)
{
	renderSources.emplace_back(source);
}

void Rendering::RemoveRenderSource(RenderSource& source)
{
	for (int i = 0; i < renderSources.size(); i++)
	{
		if (renderSources.at(i) != source)
			continue;

		renderSources.erase(renderSources.begin() + i);
	}
}