﻿
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
 
out vec2 TexCoords;
out vec4 baseColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
 
void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    baseColor = vec4(color, 1.0f);
}