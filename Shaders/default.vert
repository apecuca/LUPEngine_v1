#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTexCoord;
// Normals
layout (location = 3) in vec3 aNormal;

// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;
// Output das posi��es
out vec3 fragPos;
// Output das normals
out vec3 normals;

// Controls the scale of the vertices
//uniform float scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	// Assigns the colors from the Vertex Data to "color"
	//color = aColor;
	color = vec3(1.0, 1.0, 1.0);
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTexCoord;
	fragPos = vec3(model * vec4(aPos, 1.0));
	normals = aNormal;
}