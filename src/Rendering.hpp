#pragma once

#include <vector>

#include "glad/glad.h"

#include "RenderSource.hpp"
#include "Window.hpp"
#include "Skybox.hpp"

class Rendering
{
public:
	static void Init(bool enablePostProcessing = false);

	// Render call
	static void Render(std::shared_ptr<Window>& window, Skybox& skybox);

	// Adiciona uma nova fonte de renderização
	static void AddRenderSource(RenderSource& source);
	// Remove uma fonte de renderização
	static void RemoveRenderSource(RenderSource& source);
	// Atualiza os buffers para novo tamanho de tela
	static void ResizeFramebuffer();

	// Variables
	static bool postProcessing;

	static std::shared_ptr<Shader> GetScreenShader() { return screenShader; };

private:
	// Frame Buffer Object, Frame Buffer Texture, Render Buffer Object
	static GLuint FBO, FBT, RBO;
	static GLuint screenquadVAO, screenquadVBO;
	static std::shared_ptr<Shader> screenShader;

	// Lista de renderers
	static std::vector<std::reference_wrapper<RenderSource>> renderSources;

	// Gera todos os objetos e texturas necessárias para pós-processamento
	static void GeneratePostProcessing();
};