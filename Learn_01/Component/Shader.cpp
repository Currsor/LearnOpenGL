#include "Shader.h"

#include <glm/glm.hpp>
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

// utility uniform functions
    // ------------------------------------------------------------------------
    void Shader::setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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