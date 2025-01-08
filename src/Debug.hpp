#pragma once

#include <iostream>
#include "glm/glm.hpp"

class Debug
{
public:
	static void Log(const char* text);
	static void Log(const std::string text);
	static void Log(const int text);
	static void Log(const float text);
	static void Log(const double text);
	static void Log(const bool value);
	static void Log(const glm::vec4& value);
	static void Log(const glm::vec3& value);
	static void Log(const glm::vec2& value);

private:
	Debug();

	static void PrintTime();
};