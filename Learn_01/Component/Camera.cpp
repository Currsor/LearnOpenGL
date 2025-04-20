#include "Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "../imgui/imgui.h"           // ImGui 核心

// 构造函数(向量)
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, unsigned int WIDTH, unsigned int HEIGHT) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), 
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    lastX = WIDTH / 2.0f;
    lastY = HEIGHT / 2.0f;
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
    if (glfwGetKey(inWindow, GLFW_KEY_Q) == GLFW_PRESS)
        Position -= Up * velocity;
    if (glfwGetKey(inWindow, GLFW_KEY_E) == GLFW_PRESS)
        Position += Up * velocity;
}

// 处理鼠标移动
void Camera::ProcessMouseMovement(GLFWwindow* inWindow) {
    if (RightClickIn3DView) {
        // 当在3D View窗口内右键按下时，处理鼠标输入
        glfwSetInputMode(inWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double xpos, ypos;
        glfwGetCursorPos(inWindow, &xpos, &ypos);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(lastY - ypos); // Y轴反转
        lastX = xpos;
        lastY = ypos;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // 限制俯仰角
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        updateCameraVectors();
    } else {
        // 不在3D View窗口时恢复光标
        glfwSetInputMode(inWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

void Camera::ProcessMouseScroll(GLFWwindow* inWindow)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        // 如果 ImGui 需要捕获鼠标输入，则跳过自定义逻辑
        return;
    }
    
    glfwSetScrollCallback(inWindow, [](GLFWwindow* window, double xpos, double ypos)
    {
        static Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        cam -> MovementSpeed += ypos * 0.5f;
        if (cam -> MovementSpeed < 0.1f) cam -> MovementSpeed = 0.1f;
        if (cam -> MovementSpeed > 100.0f) cam -> MovementSpeed = 100.0f;
    });
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
