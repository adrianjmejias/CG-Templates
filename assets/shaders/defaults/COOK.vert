#version 330 core

#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
// Atributte 0 of the vertex
layout (location = 0) in vec4 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view* model* vPos;
}


