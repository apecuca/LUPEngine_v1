#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Normals
in vec3 normals;
in vec3 fragPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
//uniform sampler2D tex1;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
	/* MÚLTIPLAS TEXTURAS
	// mix (textura 1, textura 2, Alpha Blending) * Input de cor
	FragColor = mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.5) * vec4(color, 1.0);
	//
	FragColor = texture(tex0, texCoord) * vec4(lightColor * color, 1.0);
	FragColor = texture(tex0, texCoord) * vec4((0.1 * lightColor) * color, 1.0);
	*/

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    FragColor = texture(tex0, texCoord) * vec4((ambient + diffuse + specular) * color, 1.0);
}