#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(windowPtr); }
		GLFWwindow* getWindowPtr() { return windowPtr; }

		void ClearWindow();
		void SwapBuffers();

	private:
		void InitWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* windowPtr;
};