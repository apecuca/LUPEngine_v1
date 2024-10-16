#pragma once

#include "GameObject.hpp"

class Camera : public GameObject
{
protected:
    static Camera* instance;

public:
    Camera();
    // Singletons should not be cloneable
    Camera(Camera& other) = delete;
    // Singletons should not be assignable
    void operator=(const Camera&) = delete;
    // Responsável por retornar uma instância
    static Camera& GetInstance();

    float fov = 60;

    // Retorna a view matrix
    glm::mat4 GetViewMatrix();

    // Membro herdado
    void Update();

private:
    bool debugMovement;
    void DebugMovementHandler();

    glm::vec2 lastMousePosition;
};