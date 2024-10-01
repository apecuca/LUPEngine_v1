#include "LUPEngine.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	std::cout << "Hello world! Starting engine now :3..." << std::endl;
	LUPEngine app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	std::cout << "Main loop finished, stopping engine." << std::endl;

	return EXIT_SUCCESS;
}