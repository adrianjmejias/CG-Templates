#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;


uniform mat4 VP, inverseVP;

out vec4 windowPos;
out vec3 worldPos;



void main()
{
	windowPos = vec4(pos, 1);
	vec4 worldPosSheared = inverseVP * windowPos;
	worldPos = worldPosSheared.xyz / worldPosSheared.w;

	
//	vec4 test = VP * (vec4(pos,1));


	gl_Position = windowPos;
}