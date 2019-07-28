#version 330 core


#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


uniform	vec4 kD;
uniform bool isOn;

out vec4 colorsito;


void main()
{
	if(!isOn) discard;

	colorsito = kD;
}
