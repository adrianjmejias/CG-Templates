#version 330 core
#define NUM_POINT 1
#define M_PI 3.14
#define sdot(a,b) min( max(dot(a,b), 0) , 1)

struct Lightvec3{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point{
	bool isOn;
	vec3 position;
	vec3 attenuation;
	Lightvec3 vec3;
};

struct Directional{
	bool isOn;
	vec3 direction;
	Lightvec3 vec3;
};

struct Spot{
	bool isOn;
	vec3 position;
	vec3 direction;
	vec3 attenuation;
	float innerAngle;
	float outerAngle;
	Lightvec3 vec3;
};



uniform float reflectance;
uniform float shininess;
uniform float roughness;
uniform vec3 kD;
uniform vec3 kS;
uniform vec3 kA;

in vec3 o_pos;
in vec2 o_uv;
in vec3 v_norm;

uniform Point points[3];
uniform Directional directionals[1];
uniform Spot spots[1];
uniform float albedo;

out vec4 vec3;


uniform vec3 cameraPos;









float r2 = roughness*roughness;



float Fresnel(vec3 halfwayDir, vec3 viewDir){
	return reflectance + (1-reflectance) * pow(1-sdot(halfwayDir,viewDir) ,5);
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

Lightvec3 PointLight(vec3 normal, vec3 viewDir){
	Lightvec3 re = Lightvec3(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));


	int ii=0;
	// for(int ii = 0; ii < NUM_POINT; ii++){
		// if(points[ii].isOn)
		{
			float distance = length(points[ii].position - o_pos);

float attenuation =1;
			// float attenuation = 1.0f/(
			// 	(points[ii].attenuation.x)
			// 	+ (points[ii].attenuation.y *distance) 
			// 	+(points[ii].attenuation.z *distance*distance)
			// );
			
			
			re.ambient+= (attenuation * points[ii].vec3.ambient *kA);

			vec3 lightDir = normalize(points[ii].position - o_pos);
  
			float diff = sdot(normal, lightDir);
			re.diffuse+= ( diff * attenuation * points[ii].vec3.diffuse);
			// re.diffuse += diff;

			if(diff>0.f)
			{
				vec3 halfwayDir = normalize(lightDir + viewDir);	
				// float spec = pow(sdot(normal, halfwayDir),shininess);
				float spec = 1;
				float RS = Fresnel(halfwayDir,viewDir) * RoughnessFactor(normal, halfwayDir) * Geometric(normal, halfwayDir, viewDir, lightDir) / dot(normal, viewDir);
				re.specular+= (attenuation * points[ii].vec3.specular * spec) *RS* diff;
			}





		}
	// }

	return re;
}



Lightvec3 SpotLight(vec3 normal, vec3 viewDir ){
	Lightvec3 re = Lightvec3(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));

	// if(spots[0].isOn){
	// 	float distance = length(spots[0].position - o_pos);


	// 	float attenuation = 1.0f/(
	// 		(spots[0].attenuation.x)
	// 		+ (spots[0].attenuation.y *distance) 
	// 		+(spots[0].attenuation.z *distance*distance)
	// 	);


	// 	re.ambient += attenuation * spots[0].vec3.ambient;

	// 	vec3 lightDir = normalize(spots[0].position - o_pos);

	// 	float theta = sdot(lightDir,normalize(-spots[0].direction));
   	// 	float epsilon= spots[0].innerAngle - spots[0].outerAngle;
	// 	float intensity= clamp((theta - spots[0].outerAngle)/epsilon,0.,1.);


	// 	float diff = sdot(normal, lightDir);
	// 	re.diffuse += diff *spots[0].vec3.diffuse *intensity*attenuation;

	// 	if(diff > 0.f)
	// 	{
	// 		vec3 halfwayDir = normalize(lightDir + viewDir);
	// 		float spec = pow(sdot(halfwayDir, normal), shininess);	
	// 		float RS = Fresnel(halfwayDir,viewDir) * RoughnessFactor(normal, halfwayDir) * Geometric(normal, halfwayDir, viewDir, lightDir) / dot(normal, viewDir);
	// 		re.specular+= spec * intensity * attenuation * spots[0].vec3.specular*RS*diff;
	// 	}
	// }
	return re;
}




Lightvec3 DirectionalLight(vec3 normal, vec3 viewDir){
	Lightvec3 re = Lightvec3(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));

	
	// vec3 ambient=directionals[0].vec3.ambient;
	// re.ambient+=ambient *sdot(normal,directionals[0].direction);
	
	// // Direction to the light (Directional Light)
	// vec3 lightDir = normalize(-directionals[0].direction);
	// // Lambert cos(angle(Normal, Light))
	// float diff=sdot(normal,lightDir);
	// vec3 diffuse=diff*directionals[0].vec3.diffuse;
	// re.diffuse+= diff;



	//  if(diff>0.f)
	// {
    //     vec3 halfwayDir=normalize(lightDir+viewDir);
    //     float spec=pow(sdot(normal,halfwayDir),shininess);
	// 	float RS = Fresnel(halfwayDir,viewDir) * RoughnessFactor(normal, halfwayDir) * Geometric(normal, halfwayDir, viewDir, lightDir) / dot(normal, viewDir);
    //     re.specular+= directionals[0].vec3.specular*spec*RS*diff;
    // }



	return re;
}


void main()
{

	vec3 normal = normalize(v_norm);
	vec3 viewDir = normalize( cameraPos - o_pos);

	Lightvec3 IPoints = PointLight(normal, viewDir);
	Lightvec3 IDirectionals = DirectionalLight(normal,  viewDir);
	Lightvec3 ISpots = SpotLight(normal, viewDir);
	
	vec3 ambient = IPoints.ambient + IDirectionals.ambient + ISpots.ambient;
	vec3 specular = IPoints.specular + IDirectionals.specular + ISpots.specular;
	vec3 diffuse = IPoints.diffuse + IDirectionals.diffuse + ISpots.diffuse;



	// vec3 halfwayDir = normalize(lightDir + viewDir)
	// vec3 = vec4(ambient + specular+ diffuse, 1);
	vec3 = vec4(v_norm, 1);

	// vec3 = vec4(kD,0);
	// vec3 = vec4(spots[0].vec3.ambient,1);
}
