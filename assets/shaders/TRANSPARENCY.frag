#version 330 core
#define NUM_POINT 1
#define M_PI 3.14
#define sdot(a,b) min( max(dot(a,b), 0) , 1)

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform struct Material
{
	vec3 kD;
	vec3 kA;
	vec3 kS;
	vec3 kE;
	float IOR;
	float shiny;
} MAT;

uniform struct Light
{
	int type;
	vec3 position;
	vec3 attenuation;
	float innerAngle;
	float outerAngle;
	vec3 direction;
	bool isOn;
	vec3 kD;
	vec3 kA;
	vec3 kS;
} LIGHTS[1];

uniform sampler2D tex_kS;
uniform sampler2D tex_bump;
uniform sampler2D tex_kD;
uniform sampler2D tex_kA;


in struct {
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
} OBJ;

out vec4 vColor;

void main()
{

	
	vColor = texture(tex_bump, OBJ.uv.xy);

}
