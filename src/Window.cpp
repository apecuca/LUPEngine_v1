#include "Window.hpp"

#include "LUPEngine.hpp"
#include "Debug.hpp"
#include "Input.hpp"
#include "Rendering.hpp"

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

	// Configs extras
	//glfwWindowHint(GLFW_RESIZABLE, false);

	// Inicia a vari�vel window com a cria��o de uma janela glfw
	windowPtr = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

	// Define o contexto atual do glfw
	glfwMakeContextCurrent(windowPtr);

	// GLAD faz as configura��es do OpenGL
	gladLoadGL();

	// define o tamanho e origem do viewport
	glViewport(0, 0, width, height);

	// Configura��o do buffer de profundidade
	glDepthFunc(GL_LESS);

	// Stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);

	// Face culling (priorizar os da frente e em ordem counter-clockwise) 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// non-premultiplied alpha
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Callbacks
	glfwSetWindowSizeCallback(windowPtr, callback_windowResize);
}

void Window::ClearWindow()
{
	// Define a cor de fundo
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Desenha a cor de fundo por cima do buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::SwapBuffers()
{
	// Inverte os buffers de v�deo
	// (tr�s vai pra frente)
	glfwSwapBuffers(windowPtr);
}

void Window::SetDepthTest(bool vl)
{
	if (vl)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Window::callback_windowResize(GLFWwindow* window, int width, int height)
{
	// Atualizar vari�vel de tamanho
	LUPEngine::HEIGHT = height;
	LUPEngine::WIDTH = width;

	// Atualizar dimens�es do framebuffer e renderbuffer
	Rendering::ResizeFramebuffer();

	// Atualizar viewport
	glViewport(0, 0, width, height);
}