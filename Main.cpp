#include "LUPEngine.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	std::cout << "Hello world! Starting engine now :3" << std::endl;

	LUPElve::LUPEngine app{};

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}