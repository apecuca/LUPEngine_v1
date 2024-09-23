#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "Time.hpp"

class LUPEngine
{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		// Constructor e destructor
		LUPEngine();
		~LUPEngine();

		void run();

	private:
		// No futuro, trocar isso aqui pra uma vari�vel vazia
		// e iniciar ela em um m�todo Init, ou no construtor.
		// Por enquanto, a janela (vari�vel abaixo) � criada junto com este objeto
		Window window{ WIDTH, HEIGHT, "LUPEngine example" };
		Shader testShader;
		//Texture testTexture;

		//Time timeObj;
};