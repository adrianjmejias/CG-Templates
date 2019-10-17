#version 330 core




uniform mat4 projection;
uniform mat4 view;


uniform float rayStep = 0.01f;
uniform float offset = 0.01f;


struct Material;
struct Sphere;
struct Plane;
struct Ray;



void Intersection(Sphere sph, Ray ray)
{
	
}


void main()
{



	// init scene


	Ray ray = Ray();
	
	
	for(float distTraveled; ray.TTL && distTraveled < ray.reach;)
	{



	}




}


struct Material{
	float IoReflaction;
	float IoReflection;
};

struct Sphere
{
	vec3 pos;
	float radius;
};

struct Plane
{
	vec3 point;
	vec3 dir;
};

struct Ray{
	vec3 dir; 
	vec3 value; //vlaue of the last point of the ray
	int TTL; // max hits it supports
	vec4 color;
	bool hit;
	float reach; // max distance the ray can travel
};