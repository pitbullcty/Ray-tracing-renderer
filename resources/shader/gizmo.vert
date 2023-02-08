
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 color;// 为片段着色器指定一个颜色输出

uniform mat4 view;
uniform mat4 projection;

void main()
{
   
    gl_Position = projection * view * vec4(aPos, 1.0); 
    color = aColor;
}