#include "LUPEngine.hpp"

namespace LUPElve {

	void LUPEngine::run() {
		while (!lveWindow.shouldClose()) {
			// eventos de usu�rio
			glfwPollEvents();

			// simular aqui em baixo

			// desenhar na tela
			lveWindow.draw();
		}
	}

} // namespace lve