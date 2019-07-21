#version 330 core
#define color vec4
out vec4 fColor;

// Material
uniform struct
{
	color kD;
	color kA;
	color kS;
	color kE;
	float IOR;
	float shiny;
} mat;














void main()
{
    fColor = mat.kS;
}