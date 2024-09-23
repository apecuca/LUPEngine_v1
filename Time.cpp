#include "Time.hpp"

// Static vars
float Time::deltaTime = 0.0f;
float Time::currentTime = 0.0f;
float Time::previousTime = 0.0f;
float Time::scaledDeltaTime = 0.0f;
float Time::timeScale = 1.0f;

void Time::UpdateTimeVars()
{
	previousTime = currentTime;
	currentTime = static_cast<float>(glfwGetTime());
	deltaTime = currentTime - previousTime;

	scaledDeltaTime = deltaTime * timeScale;
}