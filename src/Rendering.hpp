#pragma once

#include <vector>

#include "glad/glad.h"

#include "RenderSource.hpp"

class Rendering
{
public:
	static void Init();

	// Render call
	static void Render();

	// Adiciona uma nova fonte de renderização
	static void AddRenderSource(RenderSource& source);
	// Remove uma fonte de renderização
	static void RemoveRenderSource(RenderSource& source);

private:
	// Frame buffer object
	//static GLuint FBO;
	//static GLuint screenquadVAO, screenquadVBO;

	// Lista de renderers
	static std::vector<std::reference_wrapper<RenderSource>> renderSources;
};