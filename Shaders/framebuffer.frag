#version 430 core

// Color output
out vec4 FragColor;

// In from Vertex
in vec2 TexCoords;

// Uniforms
uniform sampler2D screenTexture;
uniform int screenWidth;
uniform int screenHeight;
uniform int postprocessingFxIndex = 0;

vec3 EdgeDetection()
{
    float offset_x = 1.0 / float(screenWidth);  
    float offset_y = 1.0 / float(screenHeight);  

    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
        vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
        vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
    );

    float kernel[9] = float[]
    (
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    vec3 color = vec3(0.0f);

    for(int i = 0; i < kernel.length(); i++)
        color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];

    return color;
}

vec3 Blur(float blurStrength)
{
    // Isso basicamente só desliza o pixel, então 
    // provavelmente não é a melhor abordagem
    float offset_x = 1.0 / float(screenWidth) * blurStrength;  
    float offset_y = 1.0 / float(screenHeight) * blurStrength;  

    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
        vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
        vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
    );

    float kernel[9] = float[]
    (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 color = vec3(0.0f);

    for(int i = 0; i < kernel.length(); i++)
        color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];

    return color;
}

vec3 Greyscale()
{
    vec3 tex = texture(screenTexture, TexCoords).rgb;
    float average = 0.2126 * tex.r + 0.7152 * tex.g + 0.0722 * tex.b;

    return vec3(average);
}

vec3 ChromaticAberration()
{
    vec3 color;
    vec2 rOffset = vec2(0.01, 0.0);
    vec2 bOffset = vec2(-0.01, 0.0);

    color.r = vec3(texture(screenTexture, TexCoords - rOffset)).r;
    color.g = vec3(texture(screenTexture, TexCoords)).g;
    color.b = vec3(texture(screenTexture, TexCoords - bOffset)).b;

    return color;
}

vec3 Vignette()
{
    vec4 vignetteSettings1 = vec4(0.25);
    vec4 vignetteSettings2 = vec4(vec3(1.0), 2.5);

    vec2 viewportXY = TexCoords - 0.5;
	viewportXY = viewportXY * 2.0 - 1.0;

	vec3 vignetteXY1 = vec3(viewportXY * vignetteSettings1.xy + vignetteSettings1.zw, 1.0);
	float vignetteTerm = dot(vignetteXY1, vignetteXY1);
	float vignette = pow(vignetteTerm, vignetteSettings2.w);

	// Interpolate between the artist 'color' and white based on the physical transmission value 'vignette'.
	vec3 vignetteColor = vignetteSettings2.rgb;        
    vec3 textureColor = texture2D(screenTexture, TexCoords).rgb;
    textureColor = mix(vignetteColor, textureColor, vignette);

    return textureColor;
}

void main()
{
    vec3 color;

    switch (postprocessingFxIndex)
    {
        case 0: color = texture(screenTexture, TexCoords).rgb;
            break;
        case 1: color = Greyscale();
            break;
        case 2: color = Blur(3);
            break;
        case 3: color = vec3(1.0 - texture(screenTexture, TexCoords).rgb);
            break;
        case 4: color = EdgeDetection();
            break;
        case 5: color = ChromaticAberration();
            break;
        case 6: color = Vignette();
            break;
        default: color = vec3(1.0);
            break;
    }

    FragColor = vec4(color, 1.0);
}