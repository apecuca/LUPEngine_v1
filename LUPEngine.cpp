#include "LUPEngine.hpp"

// Inicializa�ao das vari�veis est�ticas
std::vector<GameObject> LUPEngine::instantiatedObjs;

LUPEngine::LUPEngine()
{
	// Inicializa��o
	glEnable(GL_DEPTH_TEST);

	// Updates pr�-frame
	Time::UpdateTimeVars();

	// Spawn de objetos
	LUPEngine::InstantiateObject();

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

		// Registrar os inputs
		glfwPollEvents();

		// Simular aqui em baixo
		// Atualizar as vari�veis de tempo
		Time::UpdateTimeVars();
	}
}


// Gerenciamento
GameObject& LUPEngine::InstantiateObject()
{
	instantiatedObjs.emplace_back();

	return instantiatedObjs.back();
}