#pragma once

// Standard classes
#include <vector>

// LUPE classes
#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "RenderSource.hpp"

class RenderSource;

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

		// Thread principal
		void run();

		// Gerenciamento de objetos
		// Instancia um objeto vazio
		static GameObject& InstantiateObject();
		// Destrói um objeto referenciado
		static void DestroyObject(GameObject& obj);
		// Retorna a quantidade de objetos instanciados
		static inline int GetObjectCount() { return static_cast<int>(instantiatedObjs.size()); }

		// Renderização
		// Adiciona uma nova fonte de renderização
		static void AddRenderSource(RenderSource& source);
		// Remove uma fonte de renderização
		static void RemoveRenderSource(RenderSource& source);

	private:
		// No futuro, trocar isso aqui pra um pointer
		// e iniciar ela em um método Init, ou no construtor.
		// Por enquanto, a janela (variável abaixo) é criada junto com este objeto
		//Window window{ WIDTH, HEIGHT, "LUPEngine example" };
		std::unique_ptr<Window> window;

		// Lista de objetos instancidos
		static std::vector<GameObject> instantiatedObjs;

		// Lista de renderers
		static std::vector <std::reference_wrapper<RenderSource>> renderSources;

		// Objetos para teste
		Camera camObj;
		Skybox skybox;
};