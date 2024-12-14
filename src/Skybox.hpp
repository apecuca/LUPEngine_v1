#pragma once

#include <glad/glad.h>

// Base libs
#include <iostream>

// Lib de matemática
#include <glm/glm.hpp>

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
	GLuint ID;
	GLuint VAO, VBO;
	GLuint cubemapTex;

	std::string cubemapFaces[6];

    glm::mat4 projecMat;
    glm::mat4 viewMat;

	void ConfigSkybox();
	void GenerateBufferObjs();
	void GenerateTextures();

    void SetInt(const std::string& name, int value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
};