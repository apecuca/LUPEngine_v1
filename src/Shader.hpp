#pragma once

#include <iostream>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "GameObject.hpp"

class Shader
{
public:
	Shader(
		const char* vertexFile, 
		const char* fragmentFile);
	~Shader();

    // ID do shader
	GLuint ID;

    // Bind ID
    void Use() const;

    // Compilation error checks
    static void CheckCompileErrors(GLuint shader, std::string type);

    //
    // Shader utilities
    //
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