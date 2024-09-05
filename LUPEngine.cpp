#include "LUPEngine.hpp"

LUPEngine::LUPEngine()
{
	// Start
	std::cout << "Hello world! Starting engine now :3..." << std::endl;

	// Inicialização das variáveis
	//

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
}

LUPEngine::~LUPEngine()
{
	// Fazer finalizações aqui
	//

	// Final
	std::cout << "Engine succesfully stopped. Till next time! ^w^\n";
}

void LUPEngine::run()
{
	while (!window.shouldClose()) {
		// Registrar os inputs
		glfwPollEvents();

		// Simular aqui em baixo
		//

		// Limpar fundo
		window.ClearWindow();

		// Renderizar objetos
		testShader.Render();

		// Inverter os buffers de vídeo
		window.SwapBuffers();
	}
}