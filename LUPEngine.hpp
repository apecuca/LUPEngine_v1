#pragma once

// Standard classes
#include <vector>

// LUPE classes
#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

class LUPEngine
{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		// Constructor e destructor
		LUPEngine();
		~LUPEngine();

		void run();

		// Gerenciamento de objetos
		GameObject& InstantiateObject();

	private:
		// No futuro, trocar isso aqui pra um pointer
		// e iniciar ela em um método Init, ou no construtor.
		// Por enquanto, a janela (variável abaixo) é criada junto com este objeto
		Window window{ WIDTH, HEIGHT, "LUPEngine example" };

		// Lista de objetos instancidos
		std::vector<GameObject> instantiatedObjs;
		Camera camObj;
};