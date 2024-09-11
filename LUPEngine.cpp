#include "LUPEngine.hpp"

LUPEngine::LUPEngine() : 
	testShader("Shaders/default.vert", "Shaders/default.frag")
{
	// Start
	std::cout << "Hello world! Starting engine now :3..." << std::endl;

	// Inicialização das variáveis

	// Finish startup
	std::cout << "Engine succesfully started!" << std::endl;
}

LUPEngine::~LUPEngine()
{
	// Fazer finalizações aqui
	
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
		// Registrar os inputs
		glfwPollEvents();

		// Simular aqui em baixo
		//

		// Limpar fundo
		window.ClearWindow();

		// Renderizar objetos
		testShader.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(testShader.getScaleID(), 0.5f);
		// Binds texture so that is appears in rendering
		//testTexture.Bind();
		// Bind the VAO so OpenGL knows to use it
		testShader.BindVAO();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Inverter os buffers de vídeo
		window.SwapBuffers();
	}
}