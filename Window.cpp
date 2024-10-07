#include "Window.hpp"
#include "Debug.hpp"

#include "Input.hpp"

// constructor
Window::Window(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
{
	InitWindow();
}

// destructor
Window::~Window()
{
	// Esses aqui t�o bem �bvios
	glfwDestroyWindow(windowPtr);
	glfwTerminate();
}

void Window::InitWindow() {
	// Inicia o glfw
	glfwInit();

	// Define as vers�es usadas (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Inicia a vari�vel window com a cria��o de uma janela glfw
	windowPtr = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

	// Define o contexto atual do glfw
	glfwMakeContextCurrent(windowPtr);

	// GLAD faz as configura��es do OpenGL
	gladLoadGL();

	// define o tamanho e origem do viewport
	glViewport(0, 0, width, height);
}

void Window::ClearWindow()
{
	// Define a cor de fundo
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Desenha a cor de fundo por cima do buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffers()
{
	// Inverte os buffers de v�deo
	// (tr�s vai pra frente)
	glfwSwapBuffers(windowPtr);
}