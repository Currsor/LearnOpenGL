#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

// 构造函数(向量)
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, unsigned int WIDTH, unsigned int HEIGHT) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), 
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    lastX = WIDTH;
    lastY = HEIGHT;
    updateCameraVectors();
}

// 获取视图矩阵
glm::mat4 Camera::GetViewMatrix() const
{
    return lookAt(Position, Position + Front, Up);
}

// 处理键盘输入
void Camera::ProcessKeyboard(GLFWwindow* inWindow, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (glfwGetKey(inWindow, GLFW_KEY_W) == GLFW_PRESS)
        Position += Front * velocity;
    if (glfwGetKey(inWindow, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Front * velocity;
    if (glfwGetKey(inWindow, GLFW_KEY_A) == GLFW_PRESS)
        Position -= Right * velocity;
    if (glfwGetKey(inWindow, GLFW_KEY_D) == GLFW_PRESS)
        Position += Right * velocity;
}

// 处理鼠标移动
void Camera::ProcessMouseMovement(GLFWwindow* inWindow)
{
    if (glfwGetMouseButton(inWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(inWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(inWindow, lastX, lastY);
        glfwSetCursorPosCallback(inWindow, [](GLFWwindow* window, double xpos, double ypos) {
            Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            if (cam->firstMouse) {
                cam->lastX = xpos;
                cam->lastY = ypos;
                cam->firstMouse = false;
            }
            float xoffset = xpos - cam->lastX;
            float yoffset = cam->lastY - ypos;
            cam->lastX = xpos;
            cam->lastY = ypos;
            xoffset *= cam->MouseSensitivity;
            yoffset *= cam->MouseSensitivity;

            cam->Yaw += xoffset;
            cam->Pitch += yoffset;

            // 限制俯仰角
            if (cam->Pitch > 89.0f) cam->Pitch = 89.0f;
            if (cam->Pitch < -89.0f) cam->Pitch = -89.0f;

            cam->updateCameraVectors();
        });
    }
    else
    {
        glfwSetCursorPosCallback(inWindow, nullptr);
        glfwSetInputMode(inWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    
    
}

// 更新相机向量
void Camera::updateCameraVectors() {
    // 计算新的Front向量
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = normalize(front);
    
    // 重新计算Right和Up向量
    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}

void Camera::updateFOV(GLFWwindow* inWindow)
{
    if (glfwGetMouseButton(inWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // FOV
        if (glfwGetKey(inWindow, GLFW_KEY_Z) == GLFW_PRESS)
        {
            if (Zoom < 170.0f)
            {
                Zoom += 0.5f;
                
            }
            else
            {
                Zoom = 170.0f;
            }
        }

        if (glfwGetKey(inWindow, GLFW_KEY_C) == GLFW_PRESS)
        {
            if (Zoom > 5.0f)
            {
                Zoom -= 0.5f;
            }
            else
            {
                Zoom = 5.0f;
            }
        }
    }
    else if (glfwGetMouseButton(inWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        if (Zoom != ZOOM)
        {
            if (Zoom > ZOOM)
            {
                Zoom -= 1.0f;
            }
            else if (Zoom < ZOOM)
            {
                Zoom += 1.0f;
            }
            if (fabs(Zoom - ZOOM) < 1.0f)
            {
                Zoom = ZOOM;
            }
        }
    }
}
