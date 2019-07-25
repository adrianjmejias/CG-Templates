#version 330 core
#define NUM_POINT 1
#define M_PI 3.14
#define sdot(a,b) min( max(dot(a,b), 0) , 1)

out vec4 color;

void main()
{

	
	color = vec4(1,0,0,1);

}
