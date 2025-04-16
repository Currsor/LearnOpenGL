#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 MaskValue;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform int isTargetObject; // 通过uniform区分是否是需要描边的对象

void main()
{
    FragColor = texture(skybox, TexCoords);
    MaskValue = (isTargetObject == 1) ? vec4(1.0) : vec4(0.0);
}