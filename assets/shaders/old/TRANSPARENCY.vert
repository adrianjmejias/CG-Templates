#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec4 uv;

uniform mat4 MVP;

out struct {
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
} OBJ;


void main()
{
	OBJ.uv = uv;
    gl_Position = MVP*pos;
}