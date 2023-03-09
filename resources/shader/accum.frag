
#version 330 core

in vec3 vertex;
out vec4 fragColor;

uniform sampler2D lastFrame;

void main() {
    vec3 color = texture2D(lastFrame, vertex.xy*0.5+0.5).rgb;
    fragColor = vec4(color, 1.0);
}