#pragma once

#include <glad/glad.h>

// Base libs
#include <iostream>

// Lib de matem�tica
#include <glm/glm.hpp>

std::string get_file_contents(const char* filename);

// Forward declaration
class GameObject;

// Classe para lidar com renderiza��o
class Shader
{
public:
	// Constructor com iniialia��o padr�o
	Shader(
		const GameObject& parent,
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag"
	);

	// Destructor
	~Shader();

	// M�todo para renderizar
	void Render();

private:
	const GameObject& gameObject;
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de �ndices
	GLuint ID;
	GLuint VAO, VBO, EBO;
	GLuint texture;
	
	// Transform
	glm::mat4 viewMat;
	glm::mat4 projecMat;
	glm::mat4 modelMat;

	// Mesh data
	int vertexCount;

	// Nomes dos arquivos das texturas
	const char* texFiles[3] {"default_tex.png", "brick.png", "pop_cat.png"};

	void ConfigShader(const char* vertexFile, const char* fragmentFile);
	void GenerateShader();
	// Pointer da vari�vel que vai estar a textura
	void GenerateTexture(GLuint* texVar, int fileIndex, GLenum texIndex);

	// Gerenciamento
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	glm::vec3 GetCorrectedRotation();

	// Compila e indica erros de compila��o
	void CompileErrors(GLuint shader, const char* type);
};