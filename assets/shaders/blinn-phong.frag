#version 330 core
#define M_PI 3.1415926535897932384626433832795
#define NUM_POINT 3


struct LightColor{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point{
	bool isOn;
	vec3 position;
	vec3 attenuation;
	LightColor color;
};

struct Directional{
	bool isOn;
	vec3 direction;
	LightColor color;
};

struct Spot{
	bool isOn;
	vec3 position;
	vec3 direction;
	vec3 attenuation;
	float innerAngle;
	float outerAngle;
	LightColor color;
};

in vec3 o_pos;
in vec2 o_uv;
in vec3 v_norm;

uniform Point points[3];
uniform Directional directionals[1];
uniform Spot spots[1];
uniform float albedo;
uniform float shininess;





in vec4 worldPositionalPos;

out vec4 color;


uniform vec3 cameraPos;





LightColor PointLight(vec3 normal, vec3 viewDir){
	LightColor re = LightColor(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));



	for(int ii = 0; ii < NUM_POINT; ii++){
		if(points[ii].isOn){
			float distance = length(points[ii].position - o_pos);

float attenuation= 1.0f/(
				(points[ii].attenuation.x)
				+ (points[ii].attenuation.y *distance) 
				+(points[ii].attenuation.z *distance*distance)
			);
			
			
			re.ambient+= (attenuation * points[ii].color.ambient);

			vec3 lightDir = normalize(points[ii].position - o_pos);
  
			float diff = max(dot(normal, lightDir), 0.f);
			re.diffuse+= ( diff * attenuation * points[ii].color.diffuse);
			// re.diffuse+= vec3(diff);

			if(diff>0.f)
			{

				vec3 halfwayDir = normalize(lightDir + viewDir);	
				float spec = pow(max(dot(normal, halfwayDir),0.f),shininess);
				re.specular+= (attenuation * points[ii].color.specular * spec);
			}
		}
	}

	return re;
}


LightColor SpotLight(vec3 normal, vec3 viewDir ){
	LightColor re = LightColor(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));

	if(spots[0].isOn){
		float distance = length(spots[0].position - o_pos);


		float attenuation = 1.0f/(
			(spots[0].attenuation.x)
			+ (spots[0].attenuation.y *distance) 
			+(spots[0].attenuation.z *distance*distance)
		);


		re.ambient += attenuation * spots[0].color.ambient;

		vec3 lightDir = normalize(spots[0].position - o_pos);

		float theta = dot(lightDir,normalize(spots[0].direction));
   		float epsilon= spots[0].innerAngle-spots[0].outerAngle;
		float intensity=clamp((theta-spots[0].outerAngle)/epsilon,0.,1.);


		float diff =  max(dot(normal, lightDir),0);
		re.diffuse += diff *spots[0].color.diffuse *intensity*attenuation;
		// re.diffuse+= diff * intensity;

		if(diff > 0.f)
		{
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(halfwayDir, normal), 0.f), shininess);	
			re.specular+= spec * intensity * attenuation * spots[0].color.specular;
		}
	}
	return re;
}




LightColor DirectionalLight(vec3 normal, vec3 viewDir){
	LightColor re = LightColor(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));
	if(directionals[0].isOn){

		vec3 ambient=directionals[0].color.ambient;
		re.ambient+=ambient;
		
		// Direction to the light (Directional Light)
		vec3 lightDir = normalize(-directionals[0].direction);
		// Lambert cos(angle(Normal, Light))
		float diff=max(dot(normal,lightDir),0.f);
		vec3 diffuse=diff*directionals[0].color.diffuse;
		re.diffuse+= diffuse;



		if(diff>0.f)
		{
			vec3 halfwayDir=normalize(lightDir+viewDir);
			float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
			
			re.specular+= directionals[0].color.specular*spec;
		}
	}




	return re;
}


void main()
{

	vec3 normal = normalize(v_norm);
	vec3 viewDir = normalize( cameraPos - o_pos);

	LightColor IPoints = PointLight(normal, viewDir);
	LightColor IDirectionals = DirectionalLight(normal,  viewDir);
	LightColor ISpots = SpotLight(normal, viewDir);
	
	vec3 ambient = IPoints.ambient + IDirectionals.ambient + ISpots.ambient;
	vec3 specular = IPoints.specular + IDirectionals.specular + ISpots.specular;
	vec3 diffuse = IPoints.diffuse + IDirectionals.diffuse + ISpots.diffuse;

	color = vec4(ambient + specular+ diffuse, 1);

// color = vec4(ISpots.diffuse, 1);

// color = vec4(cameraPos ,1);
	// color = vec4(spots[0].color.ambient,1);










	//diffuse + 
}
// layout (std140) uniform shader_data
// { 
//   vec3 camera_position;
//   vec3 light_position;
//   vec3 light_diffuse;
// };