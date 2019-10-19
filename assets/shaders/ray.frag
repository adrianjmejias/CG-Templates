#version 330 core


#define P2(x) (x*x)
#define sdot(x,y) max(dot(x,y),0.f)
#define PLANE 0
#define SPHERE 1

#define SCENE_SZ 2
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
	int TTL; // max hits it supports
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
		default:
		return vec3(1,0,0);

	}
}


Hit sphereIntersect(Ray ray, Object sphere) {
	vec3 c = S_CENTER(sphere);
	float r = S_RADIUS(sphere);
	
	vec3 o = ray.origin;
	vec3 d = ray.dir;

	float fB, fC;
	vec3 oc = o - c;

	fB = 2.f * dot(d, oc);
	fC =  dot(oc, oc) - P2(r);

	float disc = P2(fB) - 4*fC;
	
	if(disc < 0.0f) return Hit(false, sphere.mat, vec3(0), vec3(0), 0);
	float discSrqt = sqrt(disc);

	// hay colision
	float t = (-fB - discSrqt)/2;
	if(t < 0) t = (-fB + discSrqt)/2;
	if(t < 0) return Hit(false, sphere.mat, vec3(0), vec3(0), 0);

	vec3 hitPoint =  o + d * t;
	
	return Hit(true, sphere.mat, hitPoint, getNormal(sphere, hitPoint), t); 
}

Hit intersection(Ray ray)
{
//	Hit hit1 = sphereIntersect(ray, scene[0]);
//	Hit hit2 = sphereIntersect(ray, scene[1]);
//	return !hit1.hit || hit2.hit && hit2.t < hit1.t ? hit2 : hit1;


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
		4
	);


//	for(;;)
//	{

		Hit h = intersection(ray);
		
		if(h.hit)
		{
			acum += blinnPhong(h.mat, h.hitPoint, h.normal);

			vec3 shadowDir = lights[0].position - h.hitPoint;
			float shadowDistance = length(shadowDir);
			shadowDir = normalize(shadowDir);

			Ray shadowRay = Ray(h.hitPoint + shadowDir*offset, shadowDir, 1);
			Hit shadowHit = intersection(shadowRay);

			if(shadowHit.hit && shadowHit.t < shadowDistance)
			{
				acum*= vec3(0.1f);
			}
		}
		else
		{
			acum = vec3(0);
		}



//	}

	fragColor.xyz = acum;
	fragColor.a = 1;

//	fragColor = vec4(h.normal, 1);


//	fragColor = vec4(0,0,1,1);
//	fragColor = h.mat.color;

//fragColor=vec4(1,0,0,1);

}


void SetupScene(){

	scene[0].type = SPHERE;
	S_CENTER(scene[0]) = vec3(0,0,-2);
	S_RADIUS(scene[0]) = 1;
	scene[0].mat.colorDiff = vec3(1,0,0);
	scene[0].mat.colorSpec = vec3(0,1,0);
	scene[0].mat.IoRefraction = 1.2;
	scene[0].mat.shininess = 1;


	scene[1].type = SPHERE;
	S_CENTER(scene[1]) = vec3(4,0,-2);
	S_RADIUS(scene[1]) = 1;
	scene[1].mat.colorDiff = vec3(0,0,1);
	scene[1].mat.colorSpec = vec3(0,1,0);
	scene[1].mat.IoRefraction = 1.2;
	scene[1].mat.shininess = 1;

	lights[0].position = vec3(0,0,0);
	lights[0].att = vec3(1, 0.1f, 0.02f);
	lights[0].colorSpec = vec3(1,1,1);
	lights[0].colorDiff = vec3(1,1,1);
	
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
