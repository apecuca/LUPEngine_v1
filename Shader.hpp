#include <glad/glad.h>

#include <fstream>
#include <iostream>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Constructor e destructor
	Shader(
		const char* vertexFile = "Shaders/default.vert",
		const char* fragmentFile = "Shaders/default.frag"
	);
	~Shader();

	// Método para renderizar
	void Render();

private:
	// Coisas do OpenGL
	// Vertex Buffer Object,
	// Vertex Array Object
	// Buffer de índices
	GLuint shaderProgram;
	GLuint VAO, VBO, EBO	;

	void CompileErrors(GLuint shader, const char* type);

	void GenerateDefaultCubeData();

	// Debug
	void GenerateHelloTriangleData();
	void RenderHelloTriangle();
};