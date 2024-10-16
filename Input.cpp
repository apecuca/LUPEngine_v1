#include "Input.hpp"

#include "Debug.hpp"

// Variáveis públicas
int Input::horizontal;
int Input::vertical;
glm::vec2 Input::mousePosition;

// Privadas
std::vector<int> Input::keysPressedThisFrame;
std::vector<int> Input::keysReleasedThisFrame;
GLFWwindow* Input::winRef;

void Input::InitInput(GLFWwindow* window)
{
	winRef = window;

	glfwSetCursorPosCallback(winRef, callback_mousePos);
	glfwSetMouseButtonCallback(winRef, callback_mouseInput);
	glfwSetKeyCallback(winRef, callback_keyboard);
}

void Input::UpdateInput()
{
	keysPressedThisFrame.clear();
	keysReleasedThisFrame.clear();

	glfwPollEvents();
}

bool Input::GetKey(int key)
{
	return glfwGetKey(winRef, key);
}

bool Input::GetKeyDown(int key)
{
	for (int i = 0; i < keysPressedThisFrame.size(); i++)
	{
		if (keysPressedThisFrame.at(i) == key)
			return true;
	}

	// Se não tiver, volta falso
	return false;
}

bool Input::GetKeyUp(int key)
{
	for (int i = 0; i < keysReleasedThisFrame.size(); i++)
	{
		if (keysReleasedThisFrame.at(i) == key)
			return true;
	}

	// Se não tiver, volta falso
	return false;
}

void Input::callback_mousePos(GLFWwindow* window, double xposIn, double yposIn)
{
	mousePosition.x = static_cast<float>(xposIn);
	mousePosition.y = static_cast<float>(yposIn);
}

void Input::callback_mouseInput(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) keysPressedThisFrame.emplace_back(button);
	if (action == GLFW_RELEASE) keysReleasedThisFrame.emplace_back(button);
}

void Input::callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Debug::Log(key);

	// Adicionar e remover dos respectivos buffers
	if (action == GLFW_PRESS) keysPressedThisFrame.emplace_back(key);
	if (action == GLFW_RELEASE) keysReleasedThisFrame.emplace_back(key);

	// Atualizar axis
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS) vertical += 1;
		else if (action == GLFW_RELEASE) vertical -= 1;
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS) vertical -= 1;
		else if (action == GLFW_RELEASE) vertical += 1;
	}
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS) horizontal += 1;
		else if (action == GLFW_RELEASE) horizontal -= 1;
	}
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS) horizontal -= 1;
		else if (action == GLFW_RELEASE) horizontal += 1;
	}

	// Fechar
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(winRef, true);
}