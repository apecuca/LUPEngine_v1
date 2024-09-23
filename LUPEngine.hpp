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
		// No futuro, trocar isso aqui pra uma variável vazia
		// e iniciar ela em um método Init, ou no construtor.
		// Por enquanto, a janela (variável abaixo) é criada junto com este objeto
		Window window{ WIDTH, HEIGHT, "LUPEngine example" };
		Shader testShader;
		//Texture testTexture;

		//Time timeObj;
};