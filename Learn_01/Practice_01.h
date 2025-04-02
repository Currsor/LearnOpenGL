#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

unsigned int vao, vbo, ebo;

float mixValue = 0.2f;


// Vertex Shader
unsigned int vertexShader;
// Fragment Shader
unsigned int fragmentShader_01;
// Shader Program
unsigned int shaderProgram_01;

// 加载图片
int width, height, nrChannels;
unsigned int texture1, texture2;

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height)  // NOLINT(clang-diagnostic-shadow)
{
    return glViewport(0, 0, width, height);
}

inline void process_input(GLFWwindow *window);

