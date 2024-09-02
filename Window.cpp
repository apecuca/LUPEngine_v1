#include "Window.hpp"

namespace LUPElve {

	// constructor
	LveWindow::LveWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
		CreateTestObject();
	}

	// destructor
	LveWindow::~LveWindow() {
		// destruindo objetos de teste
		DestroyTestObject();

		// Esses aqui t�o bem �bvios
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		// Inicia o glfw
		glfwInit();

		// Define as vers�es usadas (3.3)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Inicia a vari�vel window com a cria��o de uma janela
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

		// Define o contexto atual do glfw
		glfwMakeContextCurrent(window);

		// GLAD faz as configura��es do OpenGL
		gladLoadGL();

		// define o tamanho do viewport
		glViewport(0, 0, width, height);
	}

	void LveWindow::CreateTestObject() {
		//
	}

	void LveWindow::DestroyTestObject() {
		//
	}

	void LveWindow::draw() {

		// cor do fundo
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//

		// troca o buffer de tr�s pelo da frente
		glfwSwapBuffers(window);
	}

};