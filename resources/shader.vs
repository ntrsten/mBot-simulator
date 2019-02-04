#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 OurTexCoord;
  
out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0f);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
    TexCoord = OurTexCoord;
}