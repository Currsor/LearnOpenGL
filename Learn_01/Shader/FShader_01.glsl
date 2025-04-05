#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform PointLight light;
uniform DirLight dirLight;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

uniform vec3 viewPos;


in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

void main() 
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 定向光
    // ---------------------------------------------------------
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // 点光源
    // ---------------------------------------------------------
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    // 漫反射
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    
    // 镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));
    
    // 光照衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // 应用衰减
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    // 混合纹理
    vec4 mixedTex = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    
    // 最终颜色 = (环境光 + 漫反射 + 镜面反射) * 物体颜色 * 混合纹理
    result += ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);// * mixedTex;
}