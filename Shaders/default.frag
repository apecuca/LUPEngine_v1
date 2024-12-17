#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Struct com dados da luz direcional
struct DirLight
{
    vec3 direction;
  
    vec3 ambient;
    float ambientStrength;
    vec3 diffuse;
    vec3 specular;
};

// Point light (ponto)
struct PointLight {    
    vec3 position;
    float strength;
    
    float constant;
    float linear;
    float quadratic;  

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
//uniform Light light;
uniform DirLight dirLight;
#define POINT_LIGHT_COUNT 10
uniform PointLight[POINT_LIGHT_COUNT] pointLights;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 tex)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient * light.ambientStrength * tex;
    vec3 diffuse  = light.diffuse  * diff * tex;
    vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 tex)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = tex * light.ambient * light.ambientStrength;
    vec3 diffuse  = light.diffuse  * diff * tex;
    vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ((ambient + diffuse + specular) * light.strength);
} 

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

    // properties
    vec3 norm = normalize(normals);
    vec3 viewDir = normalize(viewPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir, mixedTexture);
    // phase 2: Point lights
    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir, mixedTexture);   
    }
        
    //result += CalcPointLight(pointLight, norm, fragPos, viewDir, mixedTexture);
    
    FragColor = vec4(result, 1.0);
}

/*
struct Light {
    vec3 pointPos;

    vec3 directional;

    vec3 ambient;
    float ambientStrength;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
*/

/*
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
    //vec3 lightDir = normalize(light.position - fragPos);
    vec3 lightDir = normalize(light.pointPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * mixedTexture;  
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

    // attenuation
    float distance = length(light.pointPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;
        
    // final
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
*/