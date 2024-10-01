#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
//uniform sampler2D tex1;


void main()
{
	/*
	// mix
	// mix (textura 1, textura 2, Alpha Blending) * Input de cor
	FragColor = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.5) * vec4(color, 1.0);
	*/
	FragColor = texture(tex0, texCoord) * vec4(color, 1.0);
}