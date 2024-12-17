#pragma once

// Standard classes
#include <vector>

// LUPE classes
#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

class LUPEngine
{
	public:
		// Por favor não mexer, vai quebrar
		static int WIDTH;
		// Por favor não mexer, vai quebrar
		static int HEIGHT;

		//static constexpr int WIDTH = 800;
		//static constexpr int HEIGHT = 800;

		// Constructor e destructor
		LUPEngine();
		~LUPEngine();

		void run();

		// Gerenciamento de objetos
		static GameObject& InstantiateObject();
		static void DestroyObject(GameObject& obj);
		static inline int GetObjectCount() { return static_cast<int>(instantiatedObjs.size()); }

		// Luz
		//static glm::vec3 lightSource;

	private:
		// No futuro, trocar isso aqui pra um pointer
		// e iniciar ela em um método Init, ou no construtor.
		// Por enquanto, a janela (variável abaixo) é criada junto com este objeto
		//Window window{ WIDTH, HEIGHT, "LUPEngine example" };
		std::unique_ptr<Window> window;

		// Lista de objetos instancidos
		static std::vector<GameObject> instantiatedObjs;

		// Objetos para teste
		Camera camObj;
		Skybox skybox;
};