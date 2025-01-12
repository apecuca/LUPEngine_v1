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

LUPEngine::LUPEngine() : 
	window { std::make_unique<Window>(WIDTH, HEIGHT, "LUPEngine example") }
{
	// Inicialização
	Input::InitInput(window->getWindowPtr());
	stbi_set_flip_vertically_on_load(false);

	// Instanciar objetos
	GameObject& newObj = InstantiateObject();
	newObj.position = glm::vec3(0.0f, -0.5f, 0.0f);
	newObj.scale = glm::vec3(2.0f);
	RenderSource& objRend = *newObj.AddComponent<RenderSource>();
	objRend.ConfigShader(
		//"default.vert", "depthTest.frag", "Quirky animals/animals.gltf");
		"default.vert", "default.frag", "Quirky animals/animals.gltf");
		//"default.vert", "default.frag", "Skeleton/skeletonEmbedded.fbx");
		//"default.vert", "default.frag", "cube.fbx");
	//objRend.color.a = 0.75f;

	// light cube 1
	GameObject& lightCube1 = InstantiateObject();
	lightCube1.scale = glm::vec3(0.25f);
	lightCube1.AddComponent<RenderSource>()->ConfigShader(
		"light_cube.vert", "light_cube.frag", "cube.fbx"
	);
	Pointlight& light1 = *lightCube1.AddComponent<Pointlight>();
	light1.dir = 1.0f;
	light1.floatDist = 2.27f;

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

		// Renderizar Skybox
		skybox.Render();

		Rendering::Render();

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