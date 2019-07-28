#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


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
	vec4 kD;
	vec4 kA;
	vec4 kS;
} LIGHTS[2];

uniform sampler2D tex_kD;
uniform sampler2D tex_Bump;


in struct {
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
	vec3 bitangent;
	vec3 tangent;
} OBJ;

out vec4 colorsito;


void main()
{
	vec4 colorsito = OBJ.norm;
//	vec3 normal = normalize(OBJ.norm.xyz);

//	mat3 TBN = mat3(OBJ.tangent, OBJ.bitangent, normal);


//	for(int ii = 0; ii < LIGHTS.length(); ii++)
//	{
//		Light l = LIGHTS[ii];
//		vec3 lightDir = normalize(OBJ.w_pos.xyz - l.position);
//	
//		colorsito+= sdot(normal, lightDir) * l.kD;
//	
//	}

//	vec4 color = 




//	colorsito =texture(tex_kD, OBJ.uv.xy);
//	colorsito =vec4(OBJ.uv.xy,0,1);
}
