#version 330 core
#define P2(x) (x*x)

#define PLANE 0
#define SPHERE 1

#define SCENE_SZ 2

#define C_CENTER(x) x.geo[0]
#define C_RADIUS(x) x.geo[1][0]

#define P_CENTER(x) x.geo[0]
#define P_NORMAL(x) x.geo[1]

struct Material{
	float IoReflaction;
	float IoReflection;
	vec4 color;
};


struct Object
{
	vec3 geo[2];
	Material mat;
	int type;
};

struct Hit{
	bool hit;
	Material mat;
};

struct Ray{
	vec3 origin;
	vec3 dir; 
	vec3 value; //vlaue of the last point of the ray
	int TTL; // max hits it supports
//	vec4 color;
	bool hit;
	float reach; // max distance the ray can travel
};



uniform vec3 camPos;


uniform float rayStep = 0.01f;
uniform float offset  = 0.01f;

in vec4 worldPosSheared;
in vec4 worldPos;

out vec4 fragColor;

Object scene[SCENE_SZ];


Hit intersection(Ray ray)
{
	Hit res;
	vec3 o = ray.origin;
	vec3 d = ray.dir;
	for(int ii = 0; ii < SCENE_SZ; ii++)
	{
		if(scene[ii].type == SPHERE)
		{

//		Asumamos |Rd| = 1 (normalizado) A = 1, simplificando así la ecuación de 2do. grado.
//t0, t1 = (- B +/- (B^2 - 4*C) 1/2 ) / 2 donde t0 es para el caso (-) y t1 para el (+)
//Si el discriminante es &lt; 0.0 entonces no hay ninguna raíz real y no hay intersección. Si hay raíz real
//(Disc.&gt; = 0,0) entonces la raíz positiva menor es el punto de intersección más próximo. Así que sólo
//se puede calcular t0 y si es positiva, entonces ya está, de lo contrario calcular t1. El punto de
//intersección es:
//Ri = [xi, yi, zi] = [x0 + xd * ti , y0 + yd * ti, z0 + zd * ti]


		vec3 c = C_CENTER(scene[ii]);
		float r = C_RADIUS(scene[ii]);

		float tin, tout;
		int idxFarthest;

		float B, C;
		//	B = 2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc))
			B= 2* (d.x * (o.x - c.x) + (d.y * (o.y - c.y)) + (d.z * (o.z - c.z)));
		//	C = (X0 - Xc)^2 + (Y0 - Yc)^2 + (Z0 - Zc)^2 - Sr 2
			C = P2(o.x - c.x) + P2(o.y - c.y) + P2(o.z - c.z) - P2(r);
		}
		else if(scene[ii].type == PLANE)
		{




		}
	}

	return res;
}



void main()
{

	scene[0].type = SPHERE;
	C_CENTER(scene[0]) = vec3(0,0,10);
	C_RADIUS(scene[0]) = 2;
	scene[0].mat.color = vec4(1,0,0,1);


	scene[1].type = PLANE;
	P_CENTER(scene[1]) = vec3(0,-2,10);
	P_NORMAL(scene[1]) = vec3(0,1,0);
	scene[1].mat.color = vec4(0,0,1,1);



	vec4 worldPos = worldPosSheared/worldPosSheared.w;
	vec3 rawRay = worldPos.xyz - camPos;
//
//	// init scene
//
//
//	Ray ray = Ray(
//		normalize(rawRay),
//		worldPos,
//		1,
//		false,
//		length(rawRay)
//	);
//	
	
//	for(float distTraveled; ray.TTL && distTraveled < ray.reach;)
//	{
//
//
//
//	}

fragColor = vec4(0,length(rawRay),0,1);
fragColor.a = 1;

//fragColor=vec4(1,0,0,1);

}


