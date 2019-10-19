#version 330 core


#define P2(x) (x*x)
#define sdot(x,y) max(dot(x,y),0.f)
#define PLANE 0
#define SPHERE 1

#define SCENE_SZ 8
#define LIGHTS_SZ 1

#define S_CENTER(x) x.geo[0]
#define S_RADIUS(x) x.geo[1][0]

#define P_CENTER(x) x.geo[0]
#define P_NORMAL(x) x.geo[1]

struct Material{
	float IoRefraction;
	float IoReflection;
	vec3 colorDiff;
	vec3 colorSpec;
	float shininess;
};

struct Light 
{
	vec3 position;
	vec3 colorDiff;
	vec3 colorSpec;
	vec3 att;
};


struct Object
{
	vec3 geo[3];
	Material mat;
	int type;
};

struct Hit{
	bool hit;
	Material mat;
	vec3 hitPoint;
	vec3 normal;
	float t;
};

struct Ray{
	vec3 origin;
	vec3 dir; 
	int ttl; // max hits it supports
};



uniform vec3 camPos;


uniform float rayStep = 0.01f;
uniform float offset  = 0.01f;

in vec3 worldPos;

out vec4 fragColor;

Object scene[SCENE_SZ];
Light lights[LIGHTS_SZ];
vec3 ambientColor;
uniform float airIoR = 1;

vec3 getNormal(Object obj, vec3 hitPoint)
{
	switch(obj.type)
	{
		case SPHERE:
			return normalize(hitPoint - S_CENTER(obj));
		case PLANE:
		return P_NORMAL(obj);
		default:
		return vec3(1,0,0);

	}
}

Hit intersection(Ray ray)
{
	Hit res;

	vec3 o = ray.origin;
	vec3 d = ray.dir;
	int idxClosest = -1;
	float closestVal = 99999999;

	for(int ii = 0; ii < SCENE_SZ; ii++)
	{

	switch(scene[ii].type ){
		case SPHERE:
		
		vec3 c = S_CENTER(scene[ii]);
		float r = S_RADIUS(scene[ii]);

		float fB, fC;
		vec3 oc = o - c;

		fB = 2.f * dot(d, oc);
		fC =  dot(oc, oc) - P2(r);

		float disc = P2(fB) - 4*fC;

		if(disc >= 0)
		{
			disc = sqrt(disc);
			// hay colision
			float t = (-fB - disc)/2;
			
			if(t < 0){ 
				t = (-fB + disc)/2; 
			}
			
			if(t >= 0) // valid point
			{
				if(closestVal > t) // agarro el indice 
				{
					idxClosest = ii;
					closestVal = t;
				}
			}
		}
		break;
		case PLANE:

			vec3 n = P_NORMAL(scene[ii]);
			vec3 p = P_CENTER(scene[ii]);

		  float denom = dot(n, d); 
			if (denom > 1e-6) { 
				vec3 p0l0 = p - o; 
				float t = dot(p0l0, n) / denom; 
				if (t >= 0)
				{
					if(closestVal > t) // agarro el indice 
					{
						idxClosest = ii;
						closestVal = t;
					}
				} 
			} 

			

//				if(closestVal < t) // agarro el indice 
//				{
//					idxClosest = ii;
//					closestVal = t;
//				}
		break;
		default:
		break;
		}
	}


	
	
	if((res.hit = idxClosest >= 0))
	{
		res.mat = scene[idxClosest].mat;
		res.hitPoint = o + d * closestVal;
		res.normal = getNormal(scene[idxClosest], res.hitPoint);
	}


	return res;
}


void SetupScene();
float calcAttenuation(vec3 att, float d);
vec3 blinnPhong(Material mat, vec3 fragPos, vec3 normal)
{
	vec3 color;
	vec3 lightDir = lights[0].position - fragPos;
	float distToLight = length(lightDir);

	lightDir = normalize(lightDir);
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

//	vec3 reflectDir = reflect(ray.dir, normal);
//	vec3 refractDir = refract(ray.dir, normal, airIoR/mat.IoRefraction);
	
	float diff = sdot(viewDir,lightDir);
	float spec = pow(sdot(normal,halfwayDir), mat.shininess);
	float attenuation = calcAttenuation(lights[0].att, distToLight);

	vec3 ambient  = ambientColor.rgb  ;
	vec3 diffuse  = lights[0].colorDiff * mat.colorDiff * diff * attenuation;// * h.mat.colorDiff.rgb ;
	vec3 specular = lights[0].colorSpec * mat.colorSpec * spec * attenuation;

	return ambient + diffuse + specular;
}


void main()
{


	SetupScene();

	vec3 rayDir = normalize(worldPos - camPos);
	vec3 acum = vec3(0);
	vec3 mask = vec3(1);

	Ray ray = Ray(
		camPos,
		rayDir,
		2
	);


	for(;ray.ttl >= 0; ray.ttl--)
	{

		Hit h = intersection(ray);
		
		if(h.hit)
		{
			vec3 colorMaterial = blinnPhong(h.mat, h.hitPoint, h.normal);
			mask*=colorMaterial;
			acum+= mask ;
			vec3 shadowDir = lights[0].position - h.hitPoint;
			float shadowDistance = length(shadowDir);
			shadowDir = normalize(shadowDir);

			Ray shadowRay = Ray(h.hitPoint + shadowDir*offset, shadowDir, 1);
			Hit shadowHit = intersection(shadowRay);

			if(shadowHit.hit && shadowHit.t < shadowDistance)
			{
				acum*= vec3(0.1f);
				ray.ttl = 0;
			}
			else
			{
				ray.dir = reflect(ray.dir, h.normal);
				ray.origin = h.hitPoint  + ray.dir * offset;
			}

		}
		else
		{
			ray.ttl = 0;
//			acum = vec3(0);
		}
	}

	fragColor.xyz = acum;
	fragColor.a = 1;
}


void SetupScene(){
	int config = 0;
	scene[config].type = SPHERE;
	S_CENTER(scene[config]) = vec3(0,0,-2);
	S_RADIUS(scene[config]) = 1;
	scene[config].mat.colorDiff = vec3(1,0,0);
	scene[config].mat.colorSpec = vec3(0,0.3,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;

	config = 1;
	scene[config].type = SPHERE;
	S_CENTER(scene[config]) = vec3(4,0,-2);
	S_RADIUS(scene[config]) = 1;
	scene[config].mat.colorDiff = vec3(0,0,1);
	scene[config].mat.colorSpec = vec3(0,1,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 10;


	config = 2;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(0,0,5);
	P_NORMAL(scene[config]) = vec3(0,0,1);
	scene[config].mat.colorDiff = vec3(1,0,0);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;


	config = 3;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(0,0,-8);
	P_NORMAL(scene[config]) = vec3(0,0,-1);
	scene[config].mat.colorDiff = vec3(0,1,0);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;

	config = 4;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(5,0,0);
	P_NORMAL(scene[config]) = vec3(1,0,0);
	scene[config].mat.colorDiff = vec3(0,1,1);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;

	config = 5;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(-5,0,0);
	P_NORMAL(scene[config]) = vec3(-1,0,0);
	scene[config].mat.colorDiff = vec3(1,1,0);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;

	config = 6;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(0,5,0);
	P_NORMAL(scene[config]) = vec3(0,1,0);
	scene[config].mat.colorDiff = vec3(0,0,1);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;

	config = 7;
	scene[config].type = PLANE;
	P_CENTER(scene[config]) = vec3(0,-5,0);
	P_NORMAL(scene[config]) = vec3(0,-1,0);
	scene[config].mat.colorDiff = vec3(1,0,1);
	scene[config].mat.colorSpec = vec3(0,0,0);
	scene[config].mat.IoRefraction = 1.2;
	scene[config].mat.shininess = 1;


	config = 0;
	lights[config].position = vec3(0,2,0);
	lights[config].att = vec3(0, 0.01f, 0.002f);
	lights[config].colorSpec = vec3(1,1,1);
	lights[config].colorDiff = vec3(1,1,1);
	
	ambientColor = vec3(0.1);
}

// d for distance
// att are point light attenuation constants
float calcAttenuation(vec3 att, float d)
{
	return 1.f/(
	(att[2] * P2(d)) + // quadratic
	(att[1] * d) + //linear
	(att[0]) //constant
	);
}
