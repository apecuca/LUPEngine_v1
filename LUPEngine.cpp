#include "LUPEngine.hpp"

#include "Debug.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

// Inicializa�ao das vari�veis est�ticas
std::vector<GameObject> LUPEngine::instantiatedObjs;

LUPEngine::LUPEngine()
{
	// Inicializa��o
	glEnable(GL_DEPTH_TEST);
	Input::InitInput(window.getWindowPtr());

	// Instanciar objetos
	for (int i = 0; i < 1; i++)
	{
		GameObject& newObj = InstantiateObject();
		newObj.InitShader();
		//newObj.AddComponent<GenericComponent>();
		//newObj.position = glm::vec3(-1.25f + (1.25 * i), 0.0f, 0.0f);
	}

	// Finish startup
	Debug::Log("Engine succesfully started!");
}

LUPEngine::~LUPEngine()
{
	// Fazer finaliza��es aqui
	// Basicamente, destruir pointers e 
	// limpar arquivos tempor�rios

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

		// Inverter os buffers de v�deo
		window.SwapBuffers();

		// Atualizar vari�veis
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