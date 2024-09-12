#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <stb/stb_image.h>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Constructor e destructor
	// Constructor com inicializa��o padr�o
	Shader(
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag",
		const char* texFile = "default_tex.png"
	);
	//Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	// M�todo para renderizar
	void Render();

private:
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de �ndices
	GLuint ID;
	GLuint scaleID;
	GLuint VAO, VBO, EBO;
	GLuint texture;

	void CompileErrors(GLuint shader, const char* type);

	// Debug
	void GenerateTestData(const char* texFileName);
};