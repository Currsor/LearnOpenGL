#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

uniform vec3 viewPos;


in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;



void main() {
    // 环境光
    vec3 ambient = light.ambient * material.ambient;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // 镜面反射 (Blinn-Phong优化)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // 光照衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    
    // 应用衰减
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    // 混合纹理
    vec4 mixedTex = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    
    // 最终颜色 = (环境光 + 漫反射 + 镜面反射) * 物体颜色 * 混合纹理
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);// * mixedTex;
}