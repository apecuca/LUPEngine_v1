#pragma once

#include <glad/glad.h>

// Base libs
#include <iostream>

// Lib de matemática
#include <glm/glm.hpp>

#include "Shader.hpp"

class Skybox
{
public:
	Skybox(
		const std::string rightFile = "right.jpg",
		const std::string leftFile = "left.jpg",
		const std::string topFile = "top.jpg",
		const std::string bottomFile = "bottom.jpg",
		const std::string frontFile = "front.jpg",
		const std::string backFile = "back.jpg"
	);

	~Skybox();

	void Render();
private:
	//GLuint ID;
	std::shared_ptr<Shader> shader;
	GLuint VAO, VBO;
	GLuint cubemapTex;

	std::string cubemapFaces[6];

    glm::mat4 projecMat;
    glm::mat4 viewMat;

	//void ConfigSkybox();
	void GenerateBufferObjs();
	void GenerateTextures();
};