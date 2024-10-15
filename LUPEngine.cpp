#include "LUPEngine.hpp"

#include "Time.hpp"
#include "Input.hpp"

// Inicializa�ao das vari�veis est�ticas
//std::vector<GameObject> LUPEngine::instantiatedObjs;

LUPEngine::LUPEngine()
{
	// Inicializa��o
	glEnable(GL_DEPTH_TEST);
	Input::InitInput(window.getWindowPtr());

	// Updates pr�-frame
	Time::UpdateTimeVars();

	// Instanciar objetos e os iniciar
	LUPEngine::InstantiateObject();
	for (int i = 0; i < instantiatedObjs.size(); i++)
		instantiatedObjs.at(i).InitShader();

	// C�mera
	//instance->position.z -= 3.0f;
	camObj.position.z -= 3.f;

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
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

		// Chamar o Render do gameobject de teste
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
			instantiatedObjs.at(i).Update();
		}

		camObj.Update();
	}
}


// Gerenciamento
GameObject& LUPEngine::InstantiateObject()
{
	instantiatedObjs.emplace_back();

	return instantiatedObjs.back();
}