#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 MVP;

out vec4 TexCoords;




void main()
{
    TexCoords = aPos;
    gl_Position = MVP * aPos;
}