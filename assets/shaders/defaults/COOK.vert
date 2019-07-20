#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vUV;

// Vertex data out data
out vec3 vColor;

void main()
{
    vColor = vNormal;   
    gl_Position = vec4(vPos, 1.0f);
}