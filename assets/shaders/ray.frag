#version 330 core
#define newRay() Ray(10, )



struct Material
{
	vec4 color;
	float refractIndex;
	float reflectIndex;
};

struct Light
{
	vec3 position;
};

struct Object
{
	vec3 val[2];
	int type;
};


struct Hit
{
	float distanceTraveled;
	vec3 normal;
	vec3 point;
	Material mat;
};

struct Ray
{
	int TTL;
	vec4 color;
	float intensity;
	vec3 value;
	vec3 direction;
};

uniform float rayLimit = 300f;
uniform float offset = 0.1f;
uniform float rayStep = 0.01f;

in vec4 worldVSheared;
out vec4 fragColor;
void main()
{
	vec4 worldV = worldVSheared/worldVSheared.w;
	fragColor = vec4(1,0,0,1);
	


//	vec3 ray = vec3(gl_FragCoord.xy, 0); // cual es el z??? no puede ser el vector director;
//
//	vec3 rayDir = normalize(ray);
//
//
//	vec3 rayInc = rayDir * rayStep;
	
//	for(
		//float distTraveled = 0;
		//TTL && distTraveled < rayLimit;
		//distTraveled+=rayStep
//	) // movimiento del rayo
	{
//		for() // por cada figura en la escena
		{
			//if() // verificar intersección;
			{
			
//			break;
			}

		}






	}


}