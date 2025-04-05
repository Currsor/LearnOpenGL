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

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    glfwSetWindowUserPointer(window, &camera);

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


    // Shader
    Shader Shader_01("Shader/VShader.glsl", "Shader/FShader_01.glsl");
    Shader Shader_Light("Shader/VShader.glsl", "Shader/FShader_Light.glsl");

    // 纹理
    Texture texture1("Assets/container.jpg");
    Texture texture2("Assets/awesomeface.png");
    Texture diffuseMap("Assets/container2.png");
    Texture specularMap("Assets/container2_specular.png");


    // Mesh
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Light
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);
    glGenBuffers(1, &light_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑VBO
    glBindVertexArray(0);// 解绑VAO
    
    // 纹理参数设置
    // ---------------------------------------------------------
    texture1.SetWrap(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture1.SetWrap(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture1.SetFilter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture1.SetFilter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    texture2.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture2.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture2.SetFilter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture2.SetFilter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    diffuseMap.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
    diffuseMap.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
    diffuseMap.SetFilter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    diffuseMap.SetFilter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    specularMap.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
    specularMap.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
    specularMap.SetFilter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    
    Shader_01.use();
    Shader_01.setInt("texture1", 0);
    Shader_01.setInt("texture2", 1);
    
    Shader_01.setInt("material.diffuse", 2);
    Shader_01.setInt("material.specular", 3);
    Shader_01.setFloat("material.shininess", 32.0f);// 使用标准的 2^N 值

    // 光源衰减参数
    Shader_01.setFloat("light.constant", 1.0f);
    Shader_01.setFloat("light.linear", 0.09f);
    Shader_01.setFloat("light.quadratic", 0.032f);

    // 定向光源 - 模拟太阳光（方向从右上后方照射）
    Shader_01.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    Shader_01.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    Shader_01.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    Shader_01.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    
    
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        process_input(window);
        
        camera.ProcessKeyboard(window, deltaTime);
        camera.ProcessMouseMovement(window);
        camera.updateFOV(window);

        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    
        // 绘制线框
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // 在相应的纹理单元上绑定纹理
        texture1.Bind(GL_TEXTURE0);
        texture2.Bind(GL_TEXTURE1);
        diffuseMap.Bind(GL_TEXTURE2);
        specularMap.Bind(GL_TEXTURE3);

        Shader_01.setFloat("mixValue", mixValue);

        // 计算deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        Shader_01.use();
        
        // 创建转换
        glm::mat4 model         = glm::mat4(1.0f); // 确保首先将矩阵初始化为单位矩阵
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        
        Shader_01.setMat4("view", view);
        Shader_01.setMat4("projection", projection);
        Shader_01.setVec3("viewPos", camera.Position);

        // render container
        glBindVertexArray(vao);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 m = glm::mat4(1.0f);
            m = translate(m, cubePositions[i]);
            
            const float angle = 20.0f * i;
            
            m = rotate(m, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            // 计算法线矩阵（只在CPU执行一次）
            glm::mat3 normalMatrix = transpose(inverse(glm::mat3(m)));
    
            Shader_01.use();
            Shader_01.setMat4("model", m);
            Shader_01.setMat3("normalMatrix", normalMatrix); // 传递法线矩阵
    
            glDrawArrays(GL_TRIANGLES, 0, 36); // 批量绘制
        }

        Shader_Light.use();
        
        // 计算光源位置
        float lightAngle = glfwGetTime(); // 使用时间作为角度
        glm::mat4 lightRotation = glm::rotate(glm::mat4(1.0f), lightAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec4 rotatedLightPos = lightRotation * glm::vec4(lightPos, 1.0f);

        Shader_Light.setVec3("lightPos", glm::vec3(rotatedLightPos)); // 更新光源位置

        lightModel = glm::mat4(1.0f);
        lightModel = translate(lightModel, glm::vec3(rotatedLightPos));
        lightModel = scale(lightModel, glm::vec3(0.2f));

        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // 白色光源
        // lightColor.x = sin(glfwGetTime() * 2.0f);
        // lightColor.y = sin(glfwGetTime() * 0.7f);
        // lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f); // 很低的影响

        Shader_Light.setVec3("lightColor", lightColor);
        
        Shader_Light.setMat4("model", lightModel);
        Shader_Light.setMat4("view", view);
        Shader_Light.setMat4("projection", projection);
        
        glBindVertexArray(light_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        Shader_01.use();
        Shader_01.setVec3("light.position", rotatedLightPos);

        Shader_01.setVec3("light.ambient", ambientColor);
        Shader_01.setVec3("light.diffuse", diffuseColor);
        Shader_01.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 


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
    // ESC key to exit
    if (glfwGetKey(inWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(inWindow, true);

    // Adjust texture transparency
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
 