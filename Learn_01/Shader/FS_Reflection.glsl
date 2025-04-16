#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 MaskValue;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform int isTargetObject; // 通过uniform区分是否是需要描边的对象

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    MaskValue = (isTargetObject == 1) ? vec4(1.0) : vec4(0.0);
}