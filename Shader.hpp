#include <glad/glad.h>

// Base libs
#include <fstream>
#include <iostream>
#include <vector>

// GLFW
#include <GLFW/glfw3.h>

// Load de imagens
#include <stb/stb_image.h>

// Lib de matemática
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Classes da LUPEngine
#include "Time.hpp"

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Constructor com iniialiação padrão
	Shader(
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag"
	);
	// Destructor
	~Shader();

	// Método para renderizar
	void Render();

private:
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de índices
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
	// Pointer da variável que vai estar a textura
	void GenerateTexture(GLuint *texVar, int fileIndex, GLenum texIndex);

	// Gerenciamento
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	// Compila e indica erros de compilação
	void CompileErrors(GLuint shader, const char* type);

};