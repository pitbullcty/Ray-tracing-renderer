
#version 330 core

layout (location = 0) in vec3 pos;  // cpu传入的顶点坐标

out vec3 vertex;


void main() {
    gl_Position = vec4(pos, 1.0);
    vertex = pos;
}