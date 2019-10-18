#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

uniform mat4 VP, inverseVP;

out vec4 worldVSheared;


void main(){
	gl_Position = inverseVP * vec4(pos,1);
}