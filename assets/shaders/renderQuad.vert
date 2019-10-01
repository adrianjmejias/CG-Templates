#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_uv;
layout (location = 2) in vec3 iv_norm;


uniform sampler3D volume;
uniform mat4 projection;
uniform mat4 view;

out vec4 pos;
out vec2 texCoords;


void main()
{
	

	texCoords = i_uv;

//	pos = projection * ( view * vec4(i_pos,1));
	pos =   vec4(i_pos,1);
    gl_Position = pos;
}







