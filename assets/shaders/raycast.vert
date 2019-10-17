#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;


uniform mat4 VP, inverseVP;
out vec3 pos;



out vec4 windowPos;
out vec4 worldPos;



void main()
{
	worldPos =   (inverseVP *pos);



	gl_Position = vec4(pos, 1);
}