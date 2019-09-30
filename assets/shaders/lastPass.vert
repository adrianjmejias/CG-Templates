#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define vec3 vec4
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

layout (location = 0) in vec4 aPos;

out vec4 inPoint;
uniform mat4 MVP;

void main()
{
	vec3 a = (aPos +0.5f);
	inPoint = a;
		
    gl_Position = MVP * aPos;

}


