#version 330 core

in vec3 vertex;
layout(location = 0) out vec4 fragColor;

uniform sampler2D sourceTex;   //累积后的颜色
uniform vec2  texelSize;       //1/width, 1/height

float luminance(vec3 c) {
    return dot(c, vec3(0.2126, 0.7152, 0.0722));
}

/*
 * 方差估计: 用 3x3 邻域的一阶/二阶亮度矩估计局部噪声方差,
 * 供后续 a-trous 滤波按 "亮度差 / 标准差" 做边缘停止(即 SVGF 的做法)
 * 输出: rgb = 邻域平均颜色, a = 亮度方差
 */
void main() {
    vec2 uv = vertex.xy * 0.5 + 0.5;

    vec3  sumColor = vec3(0.0);
    float sumL = 0.0;
    float sumL2 = 0.0;
    const int n = 9;

    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            vec3 c = texture2D(sourceTex, uv + vec2(x, y) * texelSize).rgb;
            float l = luminance(c);
            sumColor += c;
            sumL  += l;
            sumL2 += l * l;
        }
    }

    float invN = 1.0 / float(n);
    vec3  meanColor = sumColor * invN;
    float meanL = sumL * invN;
    float varL = max(0.0, sumL2 * invN - meanL * meanL);

    fragColor = vec4(meanColor, varL);
}
