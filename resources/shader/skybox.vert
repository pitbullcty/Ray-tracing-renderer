
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;

    mat4 rotView = mat4(mat3(view)); // 去除旋转因子
    vec4 clipPos = projection * rotView * vec4(TexCoords, 1.0);
    clipPos.z = 0.9999 * clipPos.w;
    gl_Position = clipPos.xyzw; //尽量确保深度测试
}