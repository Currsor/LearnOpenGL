#include "Practice_01.h"

#include "Shader/Shader.h"
#include "stb_image.h"

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);

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

    // 启用Z_Buffer
    glEnable(GL_DEPTH_TEST);

    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑VBO
    glBindVertexArray(0);// 解绑VAO


    
    Shader Shader_01("Shader/VShader.glsl", "Shader/FShader_01.glsl");

    

    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
     // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// 将纹理换行设置为GL_REPEAT（默认换行方法）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 加载图像，创建纹理和生成贴图
    stbi_set_flip_vertically_on_load(true); // 告诉stb_image.h在y轴上翻转加载的纹理。
    unsigned char *data = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 加载图像，创建纹理和生成贴图
    data = stbi_load("Assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // awesomeface.png具有透明度，因此有一个alpha通道，所以一定要告诉OpenGL数据类型是GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // 告诉opengl每个采样器属于哪个纹理单元(只需要做一次)
    // -------------------------------------------------------------------------------------------
    Shader_01.use();
    Shader_01.setInt("texture1", 0);
    Shader_01.setInt("texture2", 1);
    
    
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        process_input(window);

        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    
        // 绘制
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // 在相应的纹理单元上绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        Shader_01.setFloat("mixValue", mixValue);

        Shader_01.use();
        
        // 创建转换
        glm::mat4 model         = glm::mat4(1.0f); // 确保首先将矩阵初始化为单位矩阵
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        // 变换
        view  = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        
        // 检索矩阵均匀位置
        unsigned int modelLoc = glGetUniformLocation(Shader_01.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(Shader_01.ID, "view");
        
        // 将它们传递给着色器（3种不同的方式）
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        Shader_01.setMat4("projection", projection);

        // render container
        glBindVertexArray(vao);
        glBindVertexArray(vao);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 m = glm::mat4(1.0f); // 初始化为单位矩阵
            m = translate(m, cubePositions[i]);
            if (i % 3 == 0)
            {
                const float angle = 20.0f * i; 
                m = rotate(m, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            Shader_01.setMat4("model", m);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* inWindow)
{
    if (glfwGetKey(inWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(inWindow, true);

    if (glfwGetKey(inWindow, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (mixValue < 1.0f)
        {
            mixValue += 0.001f;
        }
        else
        {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(inWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (mixValue > 0.0f)
        {
            mixValue -= 0.001f;
        }
        else
        {
            mixValue = 0.0f;
        }
    }
}
