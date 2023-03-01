
#version 330 core
in vec2 TexCoords;
in vec4 baseColor;

uniform sampler2D texture_diffuse1;
uniform bool hasTexture;
 
void main()
{
    if(hasTexture)
        gl_FragColor = baseColor * texture(texture_diffuse1, TexCoords);
    else
        gl_FragColor = baseColor;
}
