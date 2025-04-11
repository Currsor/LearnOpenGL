#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float width;
uniform float height;

void main() {
    float dx = 1.0 / width;
    float dy = 1.0 / height;

    // Sobel算子
    mat3 Gx = mat3(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    mat3 Gy = mat3(1, 2, 1, 0, 0, 0, -1, -2, -1);

    // 采样周围像素
    float s00 = texture(screenTexture, TexCoord + vec2(-dx, -dy)).r;
    float s10 = texture(screenTexture, TexCoord + vec2(0, -dy)).r;
    float s20 = texture(screenTexture, TexCoord + vec2(dx, -dy)).r;
    float s01 = texture(screenTexture, TexCoord + vec2(-dx, 0)).r;
    float s11 = texture(screenTexture, TexCoord).r; // 中心像素
    float s21 = texture(screenTexture, TexCoord + vec2(dx, 0)).r;
    float s02 = texture(screenTexture, TexCoord + vec2(-dx, dy)).r;
    float s12 = texture(screenTexture, TexCoord + vec2(0, dy)).r;
    float s22 = texture(screenTexture, TexCoord + vec2(dx, dy)).r;

    // 计算梯度
    float gx = Gx[0][0]*s00 + Gx[1][0]*s10 + Gx[2][0]*s20 +
    Gx[0][1]*s01 + Gx[1][1]*s11 + Gx[2][1]*s21 +
    Gx[0][2]*s02 + Gx[1][2]*s12 + Gx[2][2]*s22;

    float gy = Gy[0][0]*s00 + Gy[1][0]*s10 + Gy[2][0]*s20 +
    Gy[0][1]*s01 + Gy[1][1]*s11 + Gy[2][1]*s21 +
    Gy[0][2]*s02 + Gy[1][2]*s12 + Gy[2][2]*s22;

    float edge = sqrt(gx*gx + gy*gy);
    FragColor = (edge > 0.3) ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(0.0);
}