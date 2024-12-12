#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LUPEngine.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "Debug.hpp"

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
    float mouseSense = 15.f;
    glm::vec2 mousePos = Input::mousePosition;
    glm::vec2 posOffset = (glm::vec2(
        mousePos.x - lastMousePosition.x, mousePos.y - lastMousePosition.y));
    SetRotation(glm::vec3(
        rotation.x - posOffset.x * mouseSense * Time::deltaTime,
        rotation.y + posOffset.y * mouseSense * Time::deltaTime,
        rotation.z
    ), true);

    // Atualizar última posição
    lastMousePosition = mousePos;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::ScreenToWorldPoint(glm::vec2 point)
{
    // Tamanho da tela
    glm::vec2 viewSize = glm::vec2(LUPEngine::WIDTH, LUPEngine::HEIGHT);

    // Matrizes de transformação
    glm::mat4 projecMat = glm::mat4(1.0f);
    projecMat = glm::perspective(glm::radians(Camera::GetInstance().fov),
        viewSize.x / viewSize.y, 0.1f, 100.0f);
    glm::mat4 viewMat = Camera::GetInstance().GetViewMatrix();
    
    // Espaço normalizado (0...1)
    glm::vec3 ndcSpacePos = glm::vec3(point.x / viewSize.x,
        1.0f - point.y / viewSize.y, 0.0f) * 2.0f - 1.0f;

    // Coordenadas homogêneas
    glm::vec4 homogenousPos =
        glm::inverse(projecMat * viewMat) * glm::vec4(ndcSpacePos, 1.0f);

    // Coordenadas cartesianas, o ponto final no mundo
    glm::vec3 cartesianPos = glm::vec3(homogenousPos) / homogenousPos.w;

    return cartesianPos;
}

// Finalmente consegui!11!
glm::vec2 Camera::WorldToScreenPoint(glm::vec3 position)
{
    glm::vec2 viewSize = glm::vec2(LUPEngine::WIDTH, LUPEngine::HEIGHT);

    glm::mat4 projecMat = glm::mat4(1.0f);
    projecMat = glm::perspective(glm::radians(Camera::GetInstance().fov),
        viewSize.x / viewSize.y, 0.1f, 100.0f);
    glm::mat4 viewMat = Camera::GetInstance().GetViewMatrix();

    glm::vec4 clipSpacePos = projecMat * (viewMat * glm::vec4(position, 1.0f));
    glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos.x, clipSpacePos.y, clipSpacePos.z) / clipSpacePos.w;
    
    glm::vec2 screenPos = (glm::vec2(ndcSpacePos.x + 1.0f, ndcSpacePos.y + 1.0f) / 2.0f) * viewSize;

    return screenPos;
}
