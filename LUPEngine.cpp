#include "LUPEngine.hpp"

// Inicializaçao das variáveis estáticas
std::vector<GameObject> LUPEngine::instantiatedObjs;

LUPEngine::LUPEngine()
{
	// Inicialização
	glEnable(GL_DEPTH_TEST);

	// Updates pré-frame
	Time::UpdateTimeVars();

	// Spawn de objetos
	LUPEngine::InstantiateObject();

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

		// Registrar os inputs
		glfwPollEvents();

		// Simular aqui em baixo
		// Atualizar as variáveis de tempo
		Time::UpdateTimeVars();
	}
}


// Gerenciamento
GameObject& LUPEngine::InstantiateObject()
{
	instantiatedObjs.emplace_back();

	return instantiatedObjs.back();
}