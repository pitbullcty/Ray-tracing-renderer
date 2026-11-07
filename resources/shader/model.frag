
#version 330 core
in vec2 TexCoords;
in vec4 baseColor;
in vec3 worldPos;
in vec3 normal;

uniform sampler2D texture_diffuse1;
uniform bool hasTexture;
uniform vec3 cameraPos;
 
void main()
{
    vec3 albedo = baseColor.rgb;
    if(hasTexture)
        albedo *= texture(texture_diffuse1, TexCoords).rgb;

    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - worldPos);

    //双面光照:背面法线翻转,避免从内部看时整片发黑
    if(dot(N, V) < 0.0)
        N = -N;

    //主光源:跟随相机方向的头灯(略微抬起),保证任何视角下都能分辨体积与边界
    vec3 keyDir = normalize(V + vec3(0.25, 0.55, 0.0));
    float ndl = max(dot(N, keyDir), 0.0);

    //半球环境光:朝上的面偏亮、朝下的面偏暗,增强立体感
    float hemi = 0.5 + 0.5 * N.y;
    vec3 ambient = mix(vec3(0.16, 0.17, 0.20), vec3(0.42, 0.45, 0.50), hemi);

    //简单 Blinn-Phong 高光,让光滑表面边缘更清楚
    vec3 H = normalize(keyDir + V);
    float spec = pow(max(dot(N, H), 0.0), 24.0) * 0.25;

    vec3 color = albedo * (ambient + ndl * vec3(1.0, 0.98, 0.94) * 0.75) + vec3(spec);
    gl_FragColor = vec4(color, 1.0);
}
