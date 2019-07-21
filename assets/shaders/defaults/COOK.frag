#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define color vec4
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec4 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D tex_kD;
uniform sampler2D tex_kS;
uniform sampler2D tex_kA;
uniform sampler2D tex_bump;

in struct Obj{
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
} obj;

out color fColor;

void main()
{
//    vec3 dir_view = normalize(view.pos - );

	fColor = obj.norm;
}
