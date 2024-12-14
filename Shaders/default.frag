#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Struct com dados da luz
struct Light {
    vec3 position;

    vec3 ambient;
    float ambientStrength;

    vec3 diffuse;

    vec3 specular;
};

// Dados do material
struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
}; 

// Inputs the color from the Vertex Shader
// Atualmente não está sendo usada (being fed vec3(1.0))
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Normal e pos
in vec3 normals;
in vec3 fragPos;

uniform vec3 viewPos;

// Uniforms com info da luz e material
uniform Light light;
uniform Material material;

void main()
{
    // Textura principal
    // Essa junção só tá aqui por enquanto pra 
    // aprender a usar o alpha channel
    vec4 backTex = texture(material.diffuse, texCoord);
    vec4 frontTex = texture(material.specular, texCoord);
    vec3 mixedTexture = (frontTex * frontTex.a + backTex * (1 - frontTex.a)).xyz;
    // Colorizar textura
    mixedTexture *= color;

    // ambient
    vec3 ambient = mixedTexture * light.ambient * light.ambientStrength;
  	
    // diffuse 
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * mixedTexture;
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;  
        
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}