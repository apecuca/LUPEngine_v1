#pragma once

#include "Window.hpp"

namespace LUPElve {
	class LUPEngine
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		void run();

	private:
		// No futuro, trocar isso aqui pra uma vari�vel vazia
		// e iniciar ela em um m�todo Init, ou no construtor.
		// Por enquanto, a janela (vari�vel abaixo) � criada junto com este objeto
		LveWindow lveWindow{ WIDTH, HEIGHT, "LUPEngine example" };
	};
} // namespace lve