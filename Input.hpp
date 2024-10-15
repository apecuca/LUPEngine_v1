#pragma once

#include <vector>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Input
{
public:
	static int horizontal;
	static int vertical;
	static glm::vec2 mousePosition;

	static void InitInput(GLFWwindow* window);
	static void UpdateInput();
	/// Retorna se o Input está sendo pressionado.
	static bool GetKey(int key);
	/// Retorna se o Input foi pressionado no frame atual.
	static bool GetKeyDown(int key);
	/// Retorna se o Input foi solto no frame atual.
	static bool GetKeyUp(int key);
	inline static GLFWwindow* GetWindowRef() { return winRef; }

private:
	Input();

	static GLFWwindow* winRef;

	static void callback_mousePos(GLFWwindow* window, double xpos, double ypos);
	static void callback_mouseInput(GLFWwindow* window, int button, int action, int mods);
	static void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

	static std::vector<int> keysPressedThisFrame;
	static std::vector<int> keysReleasedThisFrame;
};