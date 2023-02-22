﻿
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 color;// 为片段着色器指定一个颜色输出

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
   
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    color = aColor;
}