#version 330 core

in vec3 vertex;
layout(location = 0) out vec4 fragColor;

uniform sampler2D colorTex;        //上一轮滤波结果(第一轮为累积颜色)
uniform sampler2D momentTex;       //rgb=局部平均颜色, a=亮度方差
uniform sampler2D normalDepthTex;  //xyz=主光线命中的世界法线, w=命中距离
uniform sampler2D albedoTex;       //rgb=反照率

uniform vec2  texelSize;
uniform int   stepWidth;           //a-trous 步长: 1,2,4,8 ...
uniform float sigmaNormal;         //法线相似度强度
uniform float sigmaDepth;          //深度相似度强度
uniform float sigmaLuma;           //亮度相似度强度(按方差归一)
uniform float sigmaAlbedo;         //反照率相似度强度

float luminance(vec3 c) {
    return dot(c, vec3(0.2126, 0.7152, 0.0722));
}

/*
 * 边缘保持型 a-trous(à-trous)小波滤波, 思路同 SVGF / ATrous:
 * 每轮按 2^step 的间隔做 5x5 高斯采样, 权重同时受
 *   - 法线相似度 (避免跨表面糊边)
 *   - 深度相似度 (避免前后景混色)
 *   - 反照率相似度 (保留贴图/材质边界)
 *   - 亮度相似度 (方差的倒数作为尺度, 噪声越大越平滑, 已收敛区域几乎不动)
 * 约束, 多轮迭代等效于在大范围内平滑但边界不被跨越
 */
void main() {
    ivec2 size   = textureSize(colorTex, 0) >> 0;
    ivec2 center = ivec2(gl_FragCoord.xy);

    if (center.x >= size.x || center.y >= size.y)
        discard;

    vec4  c0 = texelFetch(colorTex, center, 0);
    vec4  g0 = texelFetch(normalDepthTex, center, 0);
    vec4  m0 = texelFetch(momentTex, center, 0);
    vec3  a0 = texelFetch(albedoTex, center, 0).rgb;

    float l0      = luminance(c0.rgb);
    float stddev  = sqrt(max(m0.a, 1e-5));
    int   step    = max(stepWidth, 1);

    //天空/未命中像素没有有效法线(写入的是 (0,0,0,1e6)), 不能参与法线相似度计算,
    //否则所有邻域权重都会变成 0 -> 输出黑色。这类像素单独处理: 只在同类像素之间滤波。
    bool  sky0     = length(g0.xyz) < 0.1 || g0.w > 1.0e4;

    vec3  sumColor = vec3(0.0);
    float wsum     = 0.0;

    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            ivec2 p = center + ivec2(x, y) * step;
            if (p.x < 0 || p.y < 0 || p.x >= size.x || p.y >= size.y)
                continue;

            vec4  g   = texelFetch(normalDepthTex, p, 0);
            vec3  c   = texelFetch(colorTex, p, 0).rgb;
            vec3  alb = texelFetch(albedoTex, p, 0).rgb;

            //天空与几何表面不互相滤波
            bool  sky = length(g.xyz) < 0.1 || g.w > 1.0e4;
            if (sky != sky0)
                continue;

            //高斯空间权重
            float wSpatial = exp(-float(x * x + y * y) / 4.5);

            //法线相似度: 同类表面权重接近 1(天空无有效法线, 直接给 1)
            float wNormal = sky0 ? 1.0 : pow(max(0.0, dot(g0.xyz, g.xyz)), sigmaNormal);

            //深度相似度: 用相对差, 远处也不会过度平滑
            float depthScale = max(sigmaDepth * min(g0.w, g.w), 1e-3);
            float wDepth = exp(-abs(g0.w - g.w) / depthScale);

            //亮度相似度: 以局部标准差为尺度, 噪声大处放宽、已收敛处收紧
            float l = luminance(c);
            float wLuma = exp(-abs(l - l0) / max(sigmaLuma * stddev * float(step), 1e-4));

            //反照率相似度
            float wAlbedo = exp(-length(alb - a0) * sigmaAlbedo);

            float w = wSpatial * wNormal * wDepth * wLuma * wAlbedo;
            sumColor += c * w;
            wsum     += w;
        }
    }

    fragColor = vec4(sumColor / max(wsum, 1e-5), 1.0);
}
