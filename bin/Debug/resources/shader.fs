#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D textureColor;

void main()
{
    FragColor = texture(textureColor, TexCoord);
}
