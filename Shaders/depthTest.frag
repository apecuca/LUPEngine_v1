#version 430 core

out vec4 FragColor;

void main()
{
	// Mesmos valores que tô usando normalmente
	float near = 0.1;
	float far = 100.0;

	// Profundidade em NDC dividido por far pra ficar
	// mais fácil de ver
	float z = gl_FragCoord.z * 2.0 - 1.0;
	float depth = (2.0 * near * far) / (far + near - z * (far - near));
	depth /= (far / 2);

	FragColor = vec4(vec3(depth), 1.0);
}