#include "LUPEngine.hpp"

// Math
#include "glm/glm.hpp"

// LUPEngine 
#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "Component.hpp"

// Inicializaçao das variáveis estáticas
std::vector<GameObject> LUPEngine::instantiatedObjs;
//glm::vec3 LUPEngine::lightSource;

LUPEngine::LUPEngine()
{
	// Inicialização
	Input::InitInput(window.getWindowPtr());

	// Instanciar objetos
	for (int i = 0; i < 3; i++)
	{
		GameObject& newObj = InstantiateObject();
		newObj.InitShader(glm::clamp(LUPEngine::GetObjectCount() - 1, 0, 2));
		//newObj.scale = glm::vec3(0.2f);
		//newObj.AddComponent<GenericComponent>();
		newObj.position = glm::vec3(-1.25f + (1.25 * i), 0.0f, 0.0f);
	}

	// light cube 1
	GameObject& lightCube1 = InstantiateObject();
	lightCube1.InitShader(0, 0, "Shaders/light_cube.vert", "Shaders/light_cube.frag");
	lightCube1.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	lightCube1.AddComponent<Pointlight>()->dir = 1.0f;
	Debug::Log(lightCube1.GetComponent<Pointlight>()->GetUniqueID());

	// light cube 2
	GameObject& lightCube2 = InstantiateObject();
	lightCube2.InitShader(0, 0, "Shaders/light_cube.vert", "Shaders/light_cube.frag");
	lightCube2.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	lightCube2.AddComponent<Pointlight>()->dir = -1.0f;
	Debug::Log(lightCube2.GetComponent<Pointlight>()->GetUniqueID());

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
	while (!window.shouldClose()) {
		// Limpar fundo
		window.ClearWindow();

		// Renderizar gameobjs instanciados
		for (int i = 0; i < instantiatedObjs.size(); i++)
		{
			instantiatedObjs.at(i).Render();
		}

		// Renderizar Skybox
		//skybox.Render();
		skybox.Render();

		// Inverter os buffers de vídeo
		window.SwapBuffers();

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

// Luz
