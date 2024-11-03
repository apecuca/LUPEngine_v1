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

    // Retorna a transformação de uma coordenada no mundo para um ponto na tela
    static glm::vec2 WorldToScreenPoint(glm::vec3 position);

    // Retorna a transformação de um ponto na tela para uma coordenada no mundo
    static glm::vec3 ScreenToWorldPoint(glm::vec2 point);

    // Membro herdado
    void Update();



private:
    bool debugMovement;
    void DebugMovementHandler();

    glm::vec2 lastMousePosition;
};