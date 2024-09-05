#include "LUPEngine.hpp"

LUPEngine::LUPEngine()
{
	// Start
	std::cout << "Hello world! Starting engine now :3..." << std::endl;

	// Inicializa��o das vari�veis
	//

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
}

LUPEngine::~LUPEngine()
{
	// Fazer finaliza��es aqui
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

		// Inverter os buffers de v�deo
		window.SwapBuffers();
	}
}