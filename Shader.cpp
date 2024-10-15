#include "Shader.hpp"

// Classes da LUPEngine
#include "Time.hpp"
#include "Camera.hpp"
#include "LUPEngine.hpp"

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

Shader::Shader(const char* vertexFile, const char* fragmentFile)
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

	projecMat = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));

	// Aplicar transforms
	glUseProgram(ID);
	setMat4("model", modelMat);
	setMat4("view", viewMat);
	setMat4("projection", projecMat);

	//glUniformMatrix4fv(transformID, 1, GL_FALSE, glm::value_ptr(viewMat));
}

Shader::~Shader()
{
	// Deletar os objetos criados
	// Buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Textura
	glDeleteTextures(1, &texture);
	// Programa (shader)
    glDeleteProgram(ID);
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

void Shader::Render()
{
	// Bindar textura para ser usada
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Bindar o programa (shader) atual para ser usado 
	glUseProgram(ID);
	
	// View
	// viewMat = glm::lookAt(pos, pos + front, up);
	
	glm::vec3 camPos = Camera::GetInstance().position;
	glm::vec3 camFront = Camera::GetInstance().front;
	glm::vec3 camUp = Camera::GetInstance().up;

	projecMat = glm::perspective(glm::radians(Camera::GetInstance().fov),
		800.0f / 800.0f, 0.1f, 100.0f);
	setMat4("projection", projecMat);

	viewMat = glm::lookAt(camPos, camPos + camFront, camUp);
	setMat4("view", viewMat);

	// Model
	modelMat = glm::rotate(modelMat, glm::radians(45.0f * Time::deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
	setMat4("model", modelMat);

	// Bind o VAO para ser o próximo a ser usado
	glBindVertexArray(VAO);
	// Renderizar elemento usando os dados bindados
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
	// Vertices coordinates
	// x, y, z, CORES (RGB), Coordenadas da textura
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.5f,     1.0f, 1.0f, 1.0f,	0.0f, 1.0f, // Upper left corner
		0.5f,  0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,  // Lower right corner

		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,	0.0f, 1.0f, // Upper left corner
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};

	// Indices de renderização
	// Ordem de índices para renderização
	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2,

		3, 2, 6,
		3, 7, 6,

		7, 5, 6,
		7, 4, 5,

		4, 1, 5,
		4, 0, 1,

		1, 6, 5,
		1, 2, 6,

		4, 3, 0,
		4, 7, 3
	};

	// informações da mesh
	vertexCount = static_cast<int>((sizeof(vertices) / sizeof(GLfloat)) / 2.66f);

	// Gerar e bindar o VAO
	glGenVertexArrays(1, &VAO);
	// Bindar o VAO, sinalizando que esse é o atual
	glBindVertexArray(VAO);

	// Gerar VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Gerar EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Bind VBO > Link VBO Attributes > Unbind VBO
	// Linkar VBO com coordenadas
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Linkar VBO com cores
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Linkar VBO com coordenadas da textura
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desbindar tudo
	glBindVertexArray(0); // VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO

	//
	// TEXTURA DAQUI PRA BAIXO
	//

	GenerateTexture(&texture, 1, GL_TEXTURE0);

	// Uniform para textura
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, "tex0"), 0);
}

void Shader::GenerateTexture(GLuint *texVar, int fileIndex, GLenum texIndex)
{
	// Gerar textura 1 (pointer para variável da textura
	glGenTextures(1, texVar);
	// Binda a textura para ser usada
	glActiveTexture(texIndex);
	glBindTexture(GL_TEXTURE_2D, *texVar);
	// set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	// Depois tem que mudar isso aqui, tá mt feio
	std::string filePath = "Resources/Images/" + static_cast<std::string>(texFiles[fileIndex]);

	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	// Error handling
	if (data)
	{
		// Assigns the image to the OpenGL Texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// Generates MipMaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Limpar a memória da imagem
	stbi_image_free(data);

	// Unbind textura
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}