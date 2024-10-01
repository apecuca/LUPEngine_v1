#include "Debug.hpp"

void Debug::Log(const char* text)
{
	std::cout << text << "\n";
}

void Debug::Log(const std::string text)
{
	std::cout << text << "\n";
}

void Debug::Log(const int text)
{
	std::cout << text << "\n";
}

void Debug::Log(const float text)
{
	std::cout << text << "\n";
}