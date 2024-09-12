#include "Shader.hpp"

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

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* texFile)
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

	// Gerar Data de teste, histórico de versões
	// Hello Triangle
	// Quadrado colorido
	// > Textura
	GenerateTestData(texFile);
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
	// Bindar o programa (shader) atual para ser usado 
	glUseProgram(ID);
	// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
	glUniform1f(scaleID, 0.5f);
	// Bindar textura para ser usada
	glBindTexture(GL_TEXTURE_2D, texture);
	// Bind o VAO para ser o próximo a ser usado
	glBindVertexArray(VAO);
	// Renderizar elemento usando os dados bindados
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/// <summary>
/// DEBUG STUFF HERE
/// </summary>
void Shader::GenerateTestData(const char* texFileName)
{
	// Vertices coordinates
	// x, y, z, CORES (RGB), Coordenadas da textura
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	0.0f, 1.0f, // Upper left corner
		0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};

	// Indices de renderização
	// Ordem de índices para renderização
	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

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

	// Pegar ID da escala
	scaleID = glGetUniformLocation(ID, "scale");

	//
	// TEXTURA DAQUI PRA BAIXO
	//

	// Gerar textura (pointer para variável da textura
	glGenTextures(1, &texture);
	// Binda a textura para ser usada
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	// Depois tem que mudar isso aqui, tá mt feio
	std::string filePath = "Resources/Images/" + static_cast<std::string>(texFileName);

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

	// Uniform para textura
	GLuint texUni = glGetUniformLocation(ID, "tex0");
	glUseProgram(ID);
	glUniform1i(texUni, 0);
}