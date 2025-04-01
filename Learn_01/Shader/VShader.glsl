#version 330 core

layout(location = 0) in vec3 aPos;

uniform float offset;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x + sin(offset) * 0.2F, -aPos.y - cos(offset), aPos.z, 1.0);
    vertexColor = vec4(aPos.x + sin(offset) * 0.2F, -aPos.y -    cos(offset), aPos.z, 1.0);
}