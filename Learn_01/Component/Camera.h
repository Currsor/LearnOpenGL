#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum MovementDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // 构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
          glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
          float yaw = -90.0f, 
          float pitch = 0.0f);

    // 获取视图矩阵
    glm::mat4 GetViewMatrix() const;
    
    // 处理键盘输入
    void ProcessKeyboard(MovementDirection direction, float deltaTime);
    
    // 处理鼠标移动
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    // 处理鼠标滚轮
    void ProcessMouseScroll(float yoffset);

    // 获取投影矩阵
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    // 摄像机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // 欧拉角
    float Yaw;
    float Pitch;

    // 摄像机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

private:
    // 根据更新的欧拉角计算前向向量
    void UpdateCameraVectors();
};