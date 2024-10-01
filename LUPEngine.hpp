#pragma once

// Standard classes
#include <vector>

// LUPE classes
#include "Window.hpp"
#include "Time.hpp"
#include "GameObject.hpp"

class LUPEngine
{
	public:
		//static constexpr int WIDTH = 800;
		//static constexpr int HEIGHT = 800;

		// Constructor e destructor
		LUPEngine();
		~LUPEngine();

		void run();

		// Gerenciamento de objetos
		static GameObject& InstantiateObject();

	private:
		// No futuro, trocar isso aqui pra um pointer
		// e iniciar ela em um método Init, ou no construtor.
		// Por enquanto, a janela (variável abaixo) é criada junto com este objeto
		Window window{ 800, 800, "LUPEngine example" };

		// Lista de objetos instancidos
		static std::vector<GameObject> instantiatedObjs;
		//Shader testShader;
		//GameObject testObject;
};