
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
 
out vec2 TexCoords;
out vec4 baseColor;
out vec3 worldPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
 
void main()
{
    vec4 position = model * vec4(aPos, 1.0);
    worldPos = position.xyz;
    normal = mat3(transpose(inverse(model))) * aNormal; //法线需要用法线矩阵变换
    TexCoords = aTexCoords;
    gl_Position = projection * view * position;
    baseColor = vec4(color, 1.0f);
}
