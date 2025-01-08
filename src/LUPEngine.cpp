#include "LUPEngine.hpp"

// Math
#include "glm/glm.hpp"

// Imagem
#include "stb_image.h"

// LUPEngine 
#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "Component.hpp"

// Inicializaçao das variáveis estáticas
int LUPEngine::WIDTH = 800;
int LUPEngine::HEIGHT = 800;
std::vector<GameObject> LUPEngine::instantiatedObjs;
std::vector <std::reference_wrapper<RenderSource>> LUPEngine::renderSources;

LUPEngine::LUPEngine() : 
	window { std::make_unique<Window>(WIDTH, HEIGHT, "LUPEngine example") }
{
	// Inicialização
	Input::InitInput(window->getWindowPtr());
	stbi_set_flip_vertically_on_load(false);

	// Instanciar objetos
	GameObject& newObj2 = InstantiateObject();
	newObj2.position = glm::vec3(0.0f, -0.5f, 0.0f);
	newObj2.scale = glm::vec3(1.0f);
	newObj2.AddComponent<RenderSource>()->ConfigShader(
		"default.vert", "default.frag", "Skeleton/skeleton.gltf");

	/*
	// light cube 1
	GameObject& lightCube1 = InstantiateObject();
	lightCube1.scale = glm::vec3(0.5f);
	lightCube1.AddComponent<RenderSource>()->ConfigShader(
		"light_cube.vert", "light_cube.frag", "cube.fbx"
	);
	lightCube1.AddComponent<Pointlight>()->dir = 1.0f;
	*/

	// Finish startup
	Debug::Log("Engine succesfully started!");
}

LUPEngine::~LUPEngine()
{
	// Fazer finalizações aqui
	// Basicamente, limpar arquivos temporários

	Debug::Log("Bye bye ^w^");
}

void LUPEngine::run()
{
	while (!window->shouldClose()) {
		// Limpar fundo
		window->ClearWindow();

		// Desenhar fontes de renderização
		for (int i = 0; i < renderSources.size(); i++)
		{
			if (!renderSources.at(i).get().enabled) continue;

			renderSources.at(i).get().Draw();
		}

		// Renderizar Skybox
		skybox.Render();

		// Inverter os buffers de vídeo
		window->SwapBuffers();

		// Atualizar variáveis
		Input::UpdateInput();
		Time::UpdateTimeVars();

		// Simular objetos
		for (int i = 0; i < instantiatedObjs.size(); i++)
		{
			instantiatedObjs.at(i).UpdateBehaviour();
		}

		camObj.Update();
	}
}

// Gerenciamento
GameObject& LUPEngine::InstantiateObject()
{
	if (instantiatedObjs.size() - 1 >= instantiatedObjs.capacity())
	{
		instantiatedObjs.reserve(30);
	}

	instantiatedObjs.emplace_back();

	return instantiatedObjs.back();
}

void LUPEngine::DestroyObject(GameObject& obj)
{
	for (int i = 0; i < GetObjectCount(); i++)
	{
		if (instantiatedObjs.at(i) != obj)
			continue;
			
		instantiatedObjs.erase(instantiatedObjs.begin() + i);
	}
}

void LUPEngine::AddRenderSource(RenderSource& source)
{
	renderSources.emplace_back(source);
}

void LUPEngine::RemoveRenderSource(RenderSource& source)
{
	for (int i = 0; i < renderSources.size(); i++)
	{
		if (renderSources.at(i) != source)
			continue;

		renderSources.erase(renderSources.begin() + i);
	}
}