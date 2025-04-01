#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. 从文件读取顶点/片段着色器代码
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    // 设置文件流异常处理（失败时抛出异常）
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        // 将文件内容读入字符串流
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();

        // 将流转换为字符串
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FileReadFailure: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();  // 顶点着色器代码（C风格字符串）
    const char* fShaderCode = fragmentCode.c_str(); // 片段着色器代码

    
    // 2. 编译着色器
    unsigned int vertex, fragment;

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);                   // 创建着色器对象
    glShaderSource(vertex, 1, &vShaderCode, NULL);        // 加载源代码
    glCompileShader(vertex);                                         // 编译
    checkCompileErrors(vertex, "VERTEX");                     // 检查错误

    // 片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // 着色器程序
    ID = glCreateProgram();                             // 创建程序
    glAttachShader(ID, vertex);           // 附加顶点着色器
    glAttachShader(ID, fragment);         // 附加片段着色器
    glLinkProgram(ID);                                 // 链接
    checkCompileErrors(ID, "PROGRAM");          // 检查链接错误
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// 激活着色器程序
void Shader::use() {
    glUseProgram(ID);
}

// 设置uniform布尔值
void Shader::setBool(const std::string &name, bool value) const {
    // glUniform1i: 设置整型uniform（布尔值转换为0或1）
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

// 设置uniform整型
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// 设置uniform浮点型
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// 检查编译/链接错误
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];  // 存储错误信息的缓冲区

    if (type != "PROGRAM") {
        // 检查着色器编译错误
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::ShaderCompilationError(" << type << "):\n" 
                      << infoLog << "\n----------------------------------" << std::endl;
        }
    } else {
        // 检查程序链接错误
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::ProgramLinkError:\n" 
                      << infoLog << "\n----------------------------------" << std::endl;
        }
    }
}