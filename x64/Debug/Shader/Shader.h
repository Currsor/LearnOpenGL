#ifndef SHADER_H
#define SHader_H

#include <glad/glad.h>  // 包含OpenGL函数加载库
#include <string>       // 用于字符串操作
#include <glm/fwd.hpp>

class Shader
{
public:
    unsigned int ID;  // 着色器程序的OpenGL ID

    // 构造函数：读取并编译顶点/片段着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // 激活着色器程序
    void use();
    
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
    // 检查着色器编译/链接错误
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif