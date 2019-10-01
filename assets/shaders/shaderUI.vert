#version 330 core

//layout (location = 0) in vec3 i_pos;
//layout (location = 2) in vec3 iv_norm;
//

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 uv;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

out vec3 pos;
out vec2 TexCoords;
uniform float zIndex;


void main()
{
    TexCoords = uv;
	pos = aPos.xyz;
	vec4 modelPos =  model*aPos;
    gl_Position = vec4(modelPos.xyz, 1);
}