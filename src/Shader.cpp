#include "Shader.hpp"

// Standard
#include <fstream>

// Load de imagens
#include "stb_image.h"

// Matemática
#include <glm/gtc/matrix_transform.hpp>

// Classes da LUPEngine
#include "Camera.hpp"
#include "LUPEngine.hpp"
#include "Debug.hpp"

Shader::Shader(const GameObject& parent, const int diffuseMapIndex, const int specularMapIndex,
	const char* vertexFile, const char* fragmentFile) :
	gameObject { parent }
{
	// Configurar as variáveis do Shader
	// Vulgo criar VAO, VBO e EBO
	ConfigShader(vertexFile, fragmentFile);

	// Gerar Shader, histórico de versões
	// > Hello Triangle
	// > Quadrado colorido
	// > Textura
	// > Múltiplas (2) texturas
	GenerateShader();

	// Transform
	viewMat = glm::mat4(1.0f);
	projecMat = glm::mat4(1.0f);
	modelMat = glm::mat4(1.0f);

	//
	// !!! IMPORTANTE !!!
	// SEMPRE fazer o translate antes da rotação
	// para evitar problemas de perspectiva.
	// Se rotacioar ANTES de mudar a posição,
	// o translate moverá usando as coordenadas
	// rotacionadas como base.
	// 

	// Aplicar transforms
	glUseProgram(ID);
	SetMat4("view", viewMat);
	SetMat4("projection", projecMat);
	SetMat4("model", modelMat);

	// Light data
	glm::vec3 sunlightColor = glm::vec3(1.0f, 0.9568627f, 0.8392157f);

	SetVec3("light.ambient", sunlightColor);
	SetFloat("light.ambientStrength", 0.2f);
	SetVec3("light.diffuse", sunlightColor * glm::vec3(0.5f));
	SetVec3("light.specular", sunlightColor);
	// Directional
	SetVec3("light.directional", glm::vec3(-1.0f, -1.0f, -1.0));
	// Point light
	SetFloat("light.constant", 1.0f);
	SetFloat("light.linear", 0.09f);
	SetFloat("light.quadratic", 0.032f);

	// Material data
	GenerateTexture(&diffuseMap, diffuseMapIndex,
		"material.diffuse", 0);
	GenerateTexture(&specularMap, specularMapIndex, "material.specular", 1);
	SetFloat("material.shininess", 32.0f);
}

Shader::~Shader()
{
	//
	// Quando um programa é deletado, a memória não é necessariamente livre,
	// existem três opções que o próprio openGL decide,
	// dependendo do contexto:
	// 1 - A memória é liberada imediatamente
	// 2 - A memória é liberada em algum momento
	// 3 - A memória não é liberada, deixando o espaço livre para reuso futuro
	//

	// Deletar os objetos criados
	// Buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	// Textura
	glDeleteTextures(1, &diffuseMap);
	glDeleteTextures(1, &specularMap);
	// Programa (shader)
    glDeleteProgram(ID);
}

void Shader::Render()
{
	// Bindar o programa (shader) atual para ser usado 
	glUseProgram(ID);

	// Atualizar transforms
	// Matriz de projeção
	projecMat = glm::perspective(glm::radians(Camera::GetInstance().fov),
		(float)(LUPEngine::WIDTH) / (float)(LUPEngine::HEIGHT), 0.1f, 100.0f);
	SetMat4("projection", projecMat);

	// Matriz de visualização
	viewMat = Camera::GetInstance().GetViewMatrix();
	SetMat4("view", viewMat);
	
	// Aplicar transforms na matriz de modelo
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, gameObject.position);
	if (glm::length(gameObject.rotation) != 0)
	{
		modelMat = glm::rotate(modelMat,
			glm::radians(glm::length(gameObject.rotation)),
			glm::normalize(GetCorrectedRotation()));
	}
	modelMat = glm::scale(modelMat, gameObject.scale);

	// Atualizar matriz de modelo
	SetMat4("model", modelMat);

	// Atualizar luz
	//SetVec3("light.position", glm::vec3(
	//	sin(glfwGetTime()) * 10.0f, 5.0f, cos(glfwGetTime()) * 10.0f));
	SetVec3("light.pointPos", LUPEngine::lightSource);
	SetVec3("viewPos", Camera::GetInstance().position);

	// Bind o VAO para ser o próximo a ser usado
	glBindVertexArray(VAO);
	// Bindar textura para ser usada
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// Renderizar elemento usando os dados bindados
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Cleanup
	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	modelMat = glm::mat4(1.0f);
}

void Shader::ConfigShader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Checks if Shader compiled succesfully
	CompileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Checks if Shader compiled succesfully
	CompileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	// Checks if Shaders linked succesfully
	CompileErrors(ID, "PROGRAM");

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::GenerateShader()
{
	// Info das vertices
	const GLfloat vertices[] =
	{
		// Posição				Normals					// Tex coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0, 0.0,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0, 0.0,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0, 1.0,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0, 1.0,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0, 1.0,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0, 0.0,
		 
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0, 0.0,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0, 0.0,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0, 1.0,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0, 1.0,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0, 1.0,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0, 0.0,
		 
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0, 0.0,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0, 0.0,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0, 1.0,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0, 1.0,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0, 1.0,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0, 0.0,
		 
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0, 0.0,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0, 0.0,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0, 1.0,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0, 1.0,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0, 1.0,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0, 0.0,
		 
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0, 0.0,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0, 0.0,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0, 1.0,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0, 1.0,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0, 1.0,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0, 0.0,
		 
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0, 0.0,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0, 0.0,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0, 1.0,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0, 1.0,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0, 1.0,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0, 0.0
	};

	// Indices de renderização
	// Ordem de índices para renderização

	// informações da mesh
	vertexCount = static_cast<int>((sizeof(vertices) / sizeof(GLfloat)) / 2.66f);

	// Gerar e bindar VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Ordem de ação para VBO
	// Bind VBO > Link VBO Attributes > Unbind VBO

	// Gerar VBO e bindar VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Linkar VBO com coordenadas
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Linkar VBO com coordenadas da textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Linkar VBO com normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Desbindar tudo
	glBindVertexArray(0); // VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO
}

void Shader::GenerateTexture(GLuint *texVar, int fileIndex, const std::string uniformName, int uniformLoc)
{
	// Depois tem que mudar isso aqui, tá mt feio
	std::string filePath = "Resources/Images/" + static_cast<std::string>(texFiles[fileIndex]);

	// Gerar textura 1 (pointer para variável da textura
	glGenTextures(1, texVar);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;

	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	// Error handling
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		// Binda textura a ser usada
		glBindTexture(GL_TEXTURE_2D, *texVar);
		// Assigns the image to the OpenGL Texture object
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// Generates MipMaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind textura
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Limpar a memória da imagem
	stbi_image_free(data);

	// Uniform para textura
	glUseProgram(ID);
	SetInt(uniformName, uniformLoc);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

glm::vec3 Shader::GetCorrectedRotation()
{
	return glm::vec3(
		-gameObject.rotation.x,
		-gameObject.rotation.y,
		gameObject.rotation.z);
}

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Checks if the different Shaders have compiled properly
void Shader::CompileErrors(GLuint shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}