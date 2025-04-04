#version 330 core


uniform float mixValue;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1;
    int glossy = 32;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 specular = specularStrength * lightColor * pow(max(dot(norm, halfDir), 0.0), glossy);

    vec3 result = (ambient + diffuse + specular) * objectColor;
	// 在两个纹理之间进行线性插值（80% container, 20% awesomeface）
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) *  vec4(result, 1.0);
}