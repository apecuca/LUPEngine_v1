#include "LUPEngine.hpp"

#include "Time.hpp"
#include "Input.hpp"

// Inicializaçao das variáveis estáticas
//std::vector<GameObject> LUPEngine::instantiatedObjs;

LUPEngine::LUPEngine()
{
	// Inicialização
	glEnable(GL_DEPTH_TEST);
	Input::InitInput(window.getWindowPtr());

	// Updates pré-frame
	Time::UpdateTimeVars();

	// Instanciar objetos e os iniciar
	LUPEngine::InstantiateObject();
	for (int i = 0; i < instantiatedObjs.size(); i++)
		instantiatedObjs.at(i).InitShader();

	// Câmera
	//instance->position.z -= 3.0f;
	camObj.position.z -= 3.f;

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
}

LUPEngine::~LUPEngine()
{
	// Fazer finalizações aqui
	// Basicamente, destruir pointers e 
	// limpar arquivos temporários

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

		// Inverter os buffers de vídeo
		window.SwapBuffers();

		// Atualizar variáveis
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