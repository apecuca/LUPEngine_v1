#include "Debug.hpp"

#include <chrono>

void Debug::PrintTime()
{
	std::cout << "[" << std::chrono::system_clock::now() << "] ";
}

void Debug::Log(const char* text)
{
	PrintTime();
	std::cout << text << "\n";
}

void Debug::Log(const std::string text)
{
	PrintTime();
	std::cout << text << "\n";
}

void Debug::Log(const int text)
{
	PrintTime();
	std::cout << text << "\n";
}

void Debug::Log(const float text)
{
	PrintTime();
	std::cout << text << "\n";
}

void Debug::Log(const double text)
{
	PrintTime();
	std::cout << text << "\n";
}

void Debug::Log(const bool value)
{
	PrintTime();
	std::cout << value << "\n";
}

void Debug::Log(const glm::vec4& value)
{
	PrintTime();
	std::cout << "vec4(" << value.x << ", " << value.y << ", " << value.z << ", " << value.w << ")\n";
}

void Debug::Log(const glm::vec3& value)
{
	PrintTime();
	std::cout << "vec3(" << value.x << ", " << value.y << ", " << value.z << ")\n";
}

void Debug::Log(const glm::vec2& value)
{
	PrintTime();
	std::cout << "vec2(" << value.x << ", " << value.y << ")\n";
}