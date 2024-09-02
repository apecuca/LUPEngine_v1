#include "LUPEngine.hpp"

namespace LUPElve {

	void LUPEngine::run() {
		while (!lveWindow.shouldClose()) {
			// eventos de usuário
			glfwPollEvents();

			// simular aqui em baixo

			// desenhar na tela
			lveWindow.draw();
		}
	}

} // namespace lve