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

void Debug::Log(const double text)
{
	std::cout << text << "\n";
}

void Debug::Log(const bool value)
{
	std::cout << value << "\n";
}

void Debug::Log(const glm::vec4& value)
{
	std::cout << "vec4(" << value.x << ", " << value.y << ", " << value.z << ", " << value.w << ")\n";
}

void Debug::Log(const glm::vec3& value)
{
	std::cout << "vec3(" << value.x << ", " << value.y << ", " << value.z << ")\n";
}

void Debug::Log(const glm::vec2& value)
{
	std::cout << "vec2(" << value.x << ", " << value.y << ")\n";
}