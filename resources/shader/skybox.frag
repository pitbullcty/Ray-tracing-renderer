﻿
#version 330 core
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skybox;

vec3 toneMapping(in vec3 c, float limit) {
    float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;
    return c * 1.0 / (1.0 + luminance / limit);
}

void main()
{   
    vec3 color = texture(skybox, texCoords).rgb;
    color = toneMapping(color, 1.5);
    color = pow(color, vec3(1.0 / 2.2));
    fragColor = vec4(color, 1.0f);
}