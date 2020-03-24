#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out struct {
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
} OBJ;


void main()
{
	OBJ.m_pos = model * pos;
	OBJ.norm = normal;
	OBJ.uv = uv;


	OBJ.w_pos = projection*(view*(OBJ.m_pos));
	
    gl_Position =  OBJ.w_pos;

}