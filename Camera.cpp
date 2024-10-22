#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Time.hpp"
#include "Input.hpp"

Camera* Camera::instance = nullptr;

Camera::Camera() :
    debugMovement { false },
    lastMousePosition{ glm::vec3(0.f) }
{
    if (instance == nullptr)
    {
        instance = this;
    }
    else
    {
        delete this;
    }

    GetInstance().position = glm::vec3(0.0f, 0.0f, -4.0f);
    GetInstance().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
}

Camera& Camera::GetInstance()
{
    return *instance;
}

void Camera::Update()
{
    if (Input::GetKey(GLFW_KEY_R))
    {
        GetInstance().position = glm::vec3(0.0f, 0.0f, -4.0f);
        GetInstance().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        fov = 60.0f;
    }

    if (Input::GetKeyDown(GLFW_KEY_LEFT_ALT))
    {
        lastMousePosition = Input::mousePosition;
        debugMovement = !debugMovement;

        // Atualizar modo de Input do mouse
        glfwSetInputMode(Input::GetWindowRef(), GLFW_CURSOR,
            debugMovement ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (Input::GetKey(GLFW_KEY_DOWN))
        fov -= 20.0f * Time::deltaTime;
    if (Input::GetKey(GLFW_KEY_UP))
        fov += 20.0f * Time::deltaTime;

    if (debugMovement)
        DebugMovementHandler();
}

void Camera::DebugMovementHandler()
{
    // Movimentação
    float moveSpeed = 3.f * Time::deltaTime;

    position += right * (moveSpeed * Input::horizontal);
    position += front * (moveSpeed * Input::vertical);

    if (Input::GetKey(GLFW_KEY_SPACE))
        position += up * moveSpeed;
    if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
        position -= up * moveSpeed;

    // Rotação
    glm::vec2 mousePos = Input::mousePosition;
    glm::vec2 posOffset = (glm::vec2(
        mousePos.x - lastMousePosition.x, mousePos.y - lastMousePosition.y));
    SetRotation(glm::vec3(
        rotation.x - posOffset.x * 5.f * Time::deltaTime,
        rotation.y + posOffset.y * 5.f * Time::deltaTime,
        rotation.z
    ), true);

    // Atualizar última posição
    lastMousePosition = mousePos;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}