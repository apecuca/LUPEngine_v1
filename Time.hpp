// Impede a duplica��o de membros
#pragma once

// Base libs
#include <iostream>

// GLFW
#include <GLFW/glfw3.h>

class Time
{
public:
	// M�todo para atualizar o deltaTime
	static void UpdateTimeVars();

	// Tempo do frame atual
	static float deltaTime;
	// Escala do tempo
	static float timeScale;
	// deltaTime escalado com a timeScale, basicamente
	// um deltaTime contextualizado
	static float scaledDeltaTime;

private:
	// Vari�veis para calcular o deltaTime
	static float previousTime;
	static float currentTime;

	// Construtor privado
	// Impede cria��o de objetos
	Time();
};