#include <glad/glad.h>

#include <fstream>
#include <iostream>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Constructor e destructor
	/*
	Shader(
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag"
	);
	*/
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	// Método para renderizar
	void Activate();
	void BindVAO();
	void Render();

	GLuint getScaleID();
	GLuint getID();

private:
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de índices
	GLuint ID;
	GLuint scaleID;
	GLuint VAO, VBO, EBO;

	void CompileErrors(GLuint shader, const char* type);

	// Debug
	void GenerateTestData();
	void RenderTest();
};