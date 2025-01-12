#pragma once

// Main include
#include "Component.hpp"

// Standard
#include <iostream>
#include <vector>

// Math and opengl
#include "glm/glm.hpp"
#include "glad/glad.h"

// LUPEngine classes
#include "Model.hpp"
#include "Shader.hpp"

class RenderSource : public Component
{
public:
	RenderSource(GameObject& parent);
	~RenderSource();

	void Update() override;
	void Draw();

    // Configuration
    void ConfigShader(
        const char* vertexFile, 
        const char* fragmentFile,
        const char* modelFile);

    // Material data
    glm::vec4 color;
    float shininess;

private:
    // Shader and model pointers
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Model> model;

    // Transform matrices
    glm::mat4 projMat;
    glm::mat4 viewMat;
    glm::mat4 modelMat;

    // Corrects object rotation
    glm::vec3 GetCorrectedRotation();
};