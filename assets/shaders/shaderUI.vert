#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 model;
uniform mat4 MVP;

out vec4 TexCoords;
out float zIndex;


void main()
{
    TexCoords = aPos;
	vec4 modelPos =  model * aPos;
	zIndex = modelPos.z;
    gl_Position = vec4(modelPos.xyz, 1);
}