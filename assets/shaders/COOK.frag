#version 330 core


#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2
#define M_PI 3.14


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;


uniform struct Material
{
	vec3 kD;
	vec3 kA;
	vec3 kS;
	vec3 kE;
	float IOR;
	float shiny;
} MAT;


uniform int nLights = 0;
uniform struct Light
{
	int type;
	float innerAngle;
	float outerAngle;
	vec3 direction;
	bool isOn;


	vec3 attenuation;
	vec3 position;
	vec3 kD;
	vec3 kA;
	vec3 kS;
} LIGHTS[3];

// uniform sampler2D tex_kD;
// uniform sampler2D tex_kS;
// uniform sampler2D tex_kA;
// uniform sampler2D tex_bump;


in struct Obj{
	vec4 m_pos;
	vec4 w_pos;
	vec4 norm;
	vec4 uv;
} OBJ;

out vec4 colorsito;

uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;
uniform float nS;
uniform float nI;
uniform float roughness = 0.1f;

float r2 = roughness*roughness;

float Fresnel(vec3 halfwayDir, vec3 viewDir){
	return nS + (1-nS) * pow(1-sdot(halfwayDir,viewDir) , 5);
}

float RoughnessFactor(vec3 normal, vec3 halfwayDir){
     
	float NdH = sdot(normal,halfwayDir);
	float NdH2 = NdH *NdH;
	float NdH4 = NdH2*NdH2; 
	float Dright = exp(-((NdH2 - 1.f)/(r2* NdH2)));
	return (1.f/(M_PI*r2 * NdH4)) *Dright *Dright;
}
float Geometric(vec3 normal, vec3 halfwayDir, vec3 viewDir, vec3 lightDir){
	float NH = sdot(normal, halfwayDir);
	float VH = sdot(viewDir, halfwayDir);

	return min(1.f, min(2*NH*sdot(normal, viewDir)/VH   ,2*NH*sdot(normal, lightDir)/VH));
}


void main()
{
	vec3 normal = normalize(OBJ.norm.xyz);
	
	vec3 viewDir = OBJ.m_pos.xyz - viewPos;
	// vec3 distanceToEye = length(viewDir);
	viewDir = normalize(viewDir);

	vec3 sumDiff= vec3(0);
	vec3 sumSpec= vec3(0);
	vec3 sumAmb= vec3(0);

	for(int il = 0; il < nLights; il++)
	{
		Light l = LIGHTS[il];
		vec3 lightDir = l.position - OBJ.m_pos.xyz;
		float distance = length(lightDir);
		lightDir = normalize(lightDir);


		l.attenuation*= 0.1f;


		float attenuation = 1.0f/(
			(l.attenuation.x) +
			(l.attenuation.y *distance) +
			(l.attenuation.z *distance*distance)
		);

		float diff = sdot(normal, lightDir);
		sumDiff += ( diff * attenuation * l.kD);

		if(diff>0.f)
		{
			vec3 halfwayDir = normalize(lightDir + viewDir);	
			float spec = pow(sdot(normal, halfwayDir), nS);
			// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			// float spec = 1;
			float RS = Fresnel(halfwayDir,viewDir) * RoughnessFactor(normal, halfwayDir) * Geometric(normal, halfwayDir, viewDir, lightDir) / dot(normal, viewDir);
			sumSpec += (attenuation * l.kS * spec) * diff;
		}
		sumAmb += l.kA;
	}



	colorsito = vec4(
		// Difuso
			kD * sumDiff +
		// Specular
			kS * sumSpec +
		// Ambiental
			kA * sumAmb
		, 1);

	// colorsito = vec4(vec3(LIGHTS[0].attenuation.z) ,1);
}
