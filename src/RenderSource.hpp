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
#include "model.hpp"

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

    // Compilation error checks
    static void CheckCompileErrors(GLuint shader, std::string type);

    // Material data
    glm::vec4 color;
    float shininess;

private:
    GLuint shaderID;

    // Model pointer
    std::shared_ptr<Model> model;

    // Matrices
    glm::mat4 projMat;
    glm::mat4 viewMat;
    glm::mat4 modelMat;

    //
    // Shader utilities
    //
    void Use() const;
    glm::vec3 GetCorrectedRotation();
    // Shader uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
};