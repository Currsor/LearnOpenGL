#include "Practice_01.h"

#include "Shader/Shader.h"

int main(int argc, char* argv[])
{
    // 初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口对象
    window = glfwCreateWindow(1000, 600, "Practice", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "创建窗口对象失败!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // 调整视口大小 回调函数
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: 加载所有OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    
    // 顶点数组对象
    glGenVertexArrays(2, vao);
    
    
    // 顶点缓冲对象
    glGenBuffers(2, vbo);
    // 第一个VBO录入第一个三角形的顶点数据
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_01), vertices_01, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑VBO
    glBindVertexArray(0);// 解绑VAO_01
    
    // 第二个VBO录入第二个三角形的顶点数据
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_02), vertices_02, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑VBO
    glBindVertexArray(0);// 解绑VAO_02
    
    Shader Shader_01("Shader/VShader.glsl", "Shader/FShader_01.glsl");
    Shader Shader_02("Shader/VShader.glsl", "Shader/FShader_02.glsl");

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        process_input(window);

        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        // 绘制
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        
        Shader_01.use();
        Shader_01.setFloat("offset", timeValue);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        Shader_02.use();
        Shader_02.setFloat("offset", -0.2f);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // 释放资源
    glDeleteBuffers(1, vao);
    glDeleteBuffers(1, vbo);
    glDeleteProgram(shaderProgram_01);
    glDeleteProgram(shaderProgram_02);

    glfwTerminate();
    return 0;
}
