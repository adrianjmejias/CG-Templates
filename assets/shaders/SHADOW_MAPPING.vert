#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define vec3 vec4
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

layout (location = 0) in vec4 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out struct Obj{
	vec4 w_pos;
} OBJ;


void main()
{
	OBJ.w_pos = proj*(view*(model*pos));
    gl_Position = OBJ.w_pos;
}
