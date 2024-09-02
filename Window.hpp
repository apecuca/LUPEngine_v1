#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shader.hpp"
#include"VAO.hpp"
#include"VBO.hpp"
#include"EBO.hpp"

namespace  LUPElve {
	class LveWindow {
	public:
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		LveWindow(const LveWindow&) = delete;
		LveWindow& operator = (const LveWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		void draw();

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;

		// SOMENTE TESTES DAQUI PRA BAIXO

		GLfloat vertices[18] = {
			-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
			0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
			-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
			0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
			0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
		};

		GLuint indices[9] = {
			0, 3, 5,
			3, 2, 4,
			5, 4, 1
		};

		void CreateTestObject();
		void DestroyTestObject();
	};

} // namespace lve