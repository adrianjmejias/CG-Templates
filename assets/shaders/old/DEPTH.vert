#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 ViewProjection;

void main()
{
	gl_Position = ViewProjection * model * vec4(pos, 1.0);

}