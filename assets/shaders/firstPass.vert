#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 MVP;

out vec4 TexCoords;




void main()
{
    TexCoords = aPos +0.5f;
    gl_Position = MVP * aPos;
}