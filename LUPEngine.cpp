#include "LUPEngine.hpp"

LUPEngine::LUPEngine()
{
	// Start
	std::cout << "Hello world! Starting engine now :3..." << std::endl;

	// Inicializa��o

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
}

LUPEngine::~LUPEngine()
{
	// Fazer finaliza��es aqui
	
	// Destruir shader de teste
	testShader.~Shader();
	// Destruir Window
	window.~Window();

	// Final
	std::cout << "Engine succesfully stopped. Till next time! ^w^\n";
}

void LUPEngine::run()
{
	while (!window.shouldClose()) {
		// Limpar fundo
		window.ClearWindow();

		// Draw primitives, number of indices, datatype of indices, index of indices
		testShader.Render();

		// Inverter os buffers de v�deo
		window.SwapBuffers();

		// Registrar os inputs
		glfwPollEvents();

		// Simular aqui em baixo
		//
	}
}