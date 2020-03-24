#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define vec3 vec3
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 viewPos;

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


out struct Obj{
	vec2 uv;
	vec3 pos;
	vec3 normal;
	vec3 TangentPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
} OBJ;

void main()
{
	OBJ.uv = uv;
	OBJ.pos = vec3(model *vec4(pos,1.0f));
	
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

   mat3 TBN = mat3(T, B, N);
   mat3 iTBN = transpose(TBN);

   OBJ.TangentPos = iTBN * pos;
   OBJ.TangentLightPos = iTBN* LIGHTS[0].position;
   OBJ.TangentViewPos = iTBN* viewPos;
    gl_Position =  projection*view * model*vec4(pos,1);

}


