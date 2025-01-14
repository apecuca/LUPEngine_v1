#include "Rendering.hpp"

#include "LUPEngine.hpp"

bool Rendering::postProcessing;

std::vector <std::reference_wrapper<RenderSource>> Rendering::renderSources;
GLuint Rendering::FBO, Rendering::FBT, Rendering::RBO;
GLuint Rendering::screenquadVAO, Rendering::screenquadVBO;
std::shared_ptr<Shader> Rendering::screenShader;

/*
static GLuint FBO, TCB;
static GLuint screenquadVAO, screenquadVBO;
static std::unique_ptr<Shader> screenShader;
*/

void Rendering::Init(bool enablePostProcessing)
{
	// Modo de renderização de polígonos
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Inicializar shader da tela
	screenShader = std::make_shared<Shader>("framebuffer.vert", "framebuffer.frag");

	// Habilitar pós-processamento
	postProcessing = enablePostProcessing;

	// Lidar com pós processamento
	GeneratePostProcessing();

	// Atualizar shader
	screenShader->Use();
	// Bindar textura
	screenShader->SetInt("screenTexture", 0);
	// Variáveis inicials
	screenShader->SetInt("screenWidth", LUPEngine::WIDTH);
	screenShader->SetInt("screenHeight", LUPEngine::HEIGHT);
	screenShader->SetInt("postprocessingFxIndex", 0);
}

void Rendering::Render(std::shared_ptr<Window>& window, Skybox& skybox)
{
	if (postProcessing)
	{
		// Bindar framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	// Limpar janela
	window->ClearWindow();

	// Habilitar depth test para desenhar objetos
	glEnable(GL_DEPTH_TEST);

	// Renderizar skybox
	skybox.Render();

	// Desenhar fontes de renderização
	for (int i = 0; i < renderSources.size(); i++)
	{
		if (!renderSources.at(i).get().enabled) continue;

		renderSources.at(i).get().Draw();
	}
	
	if (postProcessing)
	{
		// Desbindar framebuffer e limpar janela
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		window->ClearWindow();

		// Desabilitar depth test para desenhar quad
		glDisable(GL_DEPTH_TEST);

		// Bindar shader e desenhar quad
		screenShader->Use();
		glBindVertexArray(screenquadVAO);
		glBindTexture(GL_TEXTURE_2D, FBT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	// Inverter os buffers de vídeo
	window->SwapBuffers();
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

void Rendering::GeneratePostProcessing()
{
	//
	// OBJETOS
	//

	// Frame buffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Gerar e configurar FBT (Frame buffer texture)
	glGenTextures(1, &FBT);
	glBindTexture(GL_TEXTURE_2D, FBT);
	// Gerar textura
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LUPEngine::WIDTH, LUPEngine::HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// Parâmetros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Atribuir a textura bindada para o frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBT, 0);

	// Gerar e configurar RBO (Render buffer object)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, LUPEngine::WIDTH, LUPEngine::HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		Debug::Log("FRAMEBUFFER ERROR:: " + std::to_string(fboStatus));

	//
	// TEXTURA e Quad
	//

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// Gerar VAO e VBO do quad da tela
	glGenVertexArrays(1, &screenquadVAO);
	glGenBuffers(1, &screenquadVBO);
	glBindVertexArray(screenquadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenquadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Desbindar tudo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rendering::ResizeFramebuffer()
{
	if (screenShader == nullptr)
		return;

	// Atualizar a textura do framebuffer (Framebuffer Texture/FBT)
	glBindTexture(GL_TEXTURE_2D, FBT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LUPEngine::WIDTH, LUPEngine::HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Atualizar objeto de renderização (Renderbuffer Object/RBO)
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, LUPEngine::WIDTH, LUPEngine::HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Desbindar usados
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Atualizar tamanho da tela no shader
	screenShader->Use();
	screenShader->SetInt("screenWidth", LUPEngine::WIDTH);
	screenShader->SetInt("screenHeight", LUPEngine::HEIGHT);
}