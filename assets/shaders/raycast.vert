#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;


uniform mat4 VP, inverseVP;

out vec4 windowPos;
out vec4 worldPosSheared;



void main()
{
	windowPos = vec4(pos, 1);
	worldPosSheared = inverseVP * windowPos;

	gl_Position = windowPos;
}