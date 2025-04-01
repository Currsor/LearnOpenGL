#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

float vertices_01[] = {
    0.8f, 0.0f, 0.0f,
    0.4f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f,
};

float vertices_02[] = {
    0.0f, 0.0f, 0.0f,
    -0.4f, -0.5f, 0.0f,
    -0.8f, 0.0f, 0.0f
};

unsigned int vbo[2];
unsigned int vao[2];

const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

const char *fragmentShaderSource_01 =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

const char *fragmentShaderSource_02 =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";

// Vertex Shader
unsigned int vertexShader;
// Fragment Shader
unsigned int fragmentShader_01;
unsigned int fragmentShader_02;
// Shader Program
unsigned int shaderProgram_01;
unsigned int shaderProgram_02;

int  success;
char infoLog[512];

inline void framebufferSizeCallback(GLFWwindow* window, int width, int height)  // NOLINT(clang-diagnostic-shadow)
{
    return glViewport(0, 0, width, height);
}

inline void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

