#include "Input.hpp"
#include "GLFW/glfw3.h"

int Input::horizontal;
int Input::vertical;

GLFWwindow* winRef;

void Input::InitInput(void* windowPtr)
{
	winRef = static_cast<GLFWwindow*>(windowPtr);
}

bool Input::GetKey(int key)
{
	return glfwGetKey(winRef, key);
}

void Input::UpdateInput()
{
	// Registrar os inputs
	glfwPollEvents();

	// Resetar variáveis
	horizontal = 0;
	vertical = 0;

	// Atualizar axis
	if (glfwGetKey(winRef, GLFW_KEY_W) == GLFW_PRESS) vertical += 1;
	if (glfwGetKey(winRef, GLFW_KEY_S) == GLFW_PRESS) vertical -= 1;
	if (glfwGetKey(winRef, GLFW_KEY_D) == GLFW_PRESS) horizontal += 1;
	if (glfwGetKey(winRef, GLFW_KEY_A) == GLFW_PRESS) horizontal -= 1;
}