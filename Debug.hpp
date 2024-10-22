#pragma once

#include <iostream>

class Debug
{
public:
	static void Log(const char* text);
	static void Log(const std::string text);
	static void Log(const int text);
	static void Log(const float text);
	static void Log(const double text);
	static void Log(const bool value);

private:
	Debug();
};