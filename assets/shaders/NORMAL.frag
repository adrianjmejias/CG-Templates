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
	float Ns;
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

uniform vec3 viewPos;

in struct {
	vec4 m_pos;
	vec4 v_pos;
	vec4 w_pos;
	vec4 v_norm;
	vec4 uv;
	vec4 tangent;
} OBJ;

out vec4 colorsito;


vec4 LigthCalc(vec3 normal, mat3 iTBN)
{
	vec3 pos = iTBN * OBJ.w_pos.xyz;
	vec3 viewPos = iTBN * viewPos;

	vec3 viewDir = normalize(viewPos - pos);


	vec4 color = vec4(0,0,0,1);

	for(int ii = 0; ii < LIGHTS.length(); ii++)
	{
		Light l = LIGHTS[ii];
		if(l.isOn){
			vec3 lightPos = iTBN * l.position;
			vec3 lightDir = normalize(lightPos-pos);
			vec3 halfway = normalize(viewDir + lightDir);
	
			color+= sdot(normal, lightDir) * l.kD;
			color+= pow(sdot(normal, halfway),MAT.Ns) * l.kS;
		}
	}
	return color;
}

vec4 Parallax(vec3 normal, mat3 iTBN)
{
	vec3 pos = iTBN * OBJ.w_pos.xyz;
	vec3 viewPos = iTBN * viewPos;

	vec3 viewDir = normalize(viewPos - pos);


	vec4 color = vec4(0,0,0,1);

	for(int ii = 0; ii < LIGHTS.length(); ii++)
	{
		Light l = LIGHTS[ii];
		if(l.isOn){
			vec3 lightPos = iTBN * l.position;
			vec3 lightDir = normalize(lightPos-pos);
			vec3 halfway = normalize(viewDir + lightDir);
	
			color+= sdot(normal, lightDir) * l.kD;
			color+= pow(sdot(normal, halfway),MAT.Ns) * l.kS;
		}
	}
	return color;
}

void main()
{
//	colorsito = texture(tex_kD, OBJ.uv.xy);

	vec3 normal = texture(tex_kD ,OBJ.uv.xy).xyz;
	mat3 TBN = mat3(OBJ.tangent.xyz, cross(OBJ.tangent.xyz, normal), normal);
	mat3 iTBN = transpose(TBN);

	colorsito = LigthCalc(TBN * normal, mat3(1));
}
