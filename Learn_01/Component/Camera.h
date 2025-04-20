#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 默认相机参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 50.0f;

class Camera {
public:
    // 相机属性
    glm::vec3 Position;  // 相机位置
    glm::vec3 Front;     // 相机前方向量
    glm::vec3 Up;        // 相机上向量
    glm::vec3 Right;     // 相机右向量
    glm::vec3 WorldUp;   // 世界上向量
    
    // 欧拉角
    float Yaw = -90.0f;  // 偏航角
    float Pitch;         // 俯仰角
    
    // 相机选项
    float MovementSpeed; // 移动速度
    float MouseSensitivity; // 鼠标灵敏度
    float Zoom;          // 视野范围
    float lastX, lastY;  // 鼠标位置
    bool firstMouse = true;
    bool RightClickIn3DView; // 右键点击3D视图标志
    
    // 构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH, unsigned int WIDTH = 800, unsigned int HEIGHT = 600);
    
    // 获取视图矩阵
    glm::mat4 GetViewMatrix() const;
    
    // 处理键盘输入
    void ProcessKeyboard(GLFWwindow* inWindow, float deltaTime);

    // 处理鼠标移动
    void ProcessMouseMovement(GLFWwindow* inWindow);

    // 处理鼠标滚轮
    void ProcessMouseScroll(GLFWwindow* inWindow);
    
    // 更新FOV
    void updateFOV(GLFWwindow* inWindow);
private:
    // 更新相机向量
    void updateCameraVectors();

};