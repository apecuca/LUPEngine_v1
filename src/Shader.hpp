#pragma once

#include <glad/glad.h>

// Base libs
#include <iostream>

// Lib de matemática
#include <glm/glm.hpp>

std::string get_file_contents(const char* filename);

// Forward declaration
class GameObject;

// Classe para lidar com renderização
class Shader
{
public:
	// Constructor com iniialiação padrão
	Shader(
		const GameObject& parent,

		const int diffuseMapIndex = 0,
		const int specularMapIndex = 0,

		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag"
	);

	// Destructor
	~Shader();

	// Método para renderizar
	void Render();

	// Compila e indica erros de compilação
	static void CompileErrors(GLuint shader, const char* type);

private:
	const GameObject& gameObject;
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de índices
	GLuint ID;
	GLuint VAO, VBO; //, EBO;
	//GLuint texture;
	GLuint diffuseMap;
	GLuint specularMap;
	
	// Transform
	glm::mat4 viewMat;
	glm::mat4 projecMat;
	glm::mat4 modelMat;

	// Mesh data
	int vertexCount;

	// Nomes dos arquivos das texturas
	const std::string texFiles[4] =
	{
		"default_tex.png",
		"brick.png",
		"pop_cat.png",
		"container_specular.png"
	};

	void ConfigShader(const char* vertexFile, const char* fragmentFile);
	void GenerateShader();
	// Pointer da variável que vai estar a textura
	void GenerateTexture(GLuint* texVar, int fileIndex, const std::string uniformName, int uniformLoc);

	// Gerenciamento
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	glm::vec3 GetCorrectedRotation();
};