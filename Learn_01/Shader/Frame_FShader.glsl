#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D maskTexture;
uniform vec2 textureSize; // 纹理尺寸（宽度,高度）
uniform float edgeThreshold = 0.3; // 边缘检测阈值

void main()
{
    // 计算单个像素的偏移量
    vec2 texelSize = 1.0 / textureSize;

    // Sobel算子卷积核（水平和垂直方向）
    float sobelKernelX[9] = float[]
    (
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    );
    float sobelKernelY[9] = float[]
    (
        -1, -2, -1,
        0,  0,  0,
        1,  2,  1
    );

    // 采样周围3x3像素
    float gradientX = 0.0;
    float gradientY = 0.0;
    for (int i = -1; i <= 1; i++) 
    {
        for (int j = -1; j <= 1; j++) 
        {
            vec2 offset = vec2(i, j) * texelSize * 2.0;
            float R = texture(maskTexture, TexCoords + offset).r; // 取遮罩图的红色通道（黑白图）
            int index = (i + 1) * 3 + (j + 1);
            gradientX += R * sobelKernelX[index];
            gradientY += R * sobelKernelY[index];
        }
    }

    // 计算梯度幅值
    float edgeStrength = sqrt(gradientX * gradientX + gradientY * gradientY);

    // 根据阈值生成描边（黑色边缘，白色背景）
    if (edgeStrength > edgeThreshold) 
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // 边缘为黑色
    } 
    else 
    {
        FragColor = texture(screenTexture, TexCoords);
    }
}