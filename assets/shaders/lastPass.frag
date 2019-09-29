#version 330 core


#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


vec2 mapCoords();

uniform vec2 windowSize;
uniform sampler2D bfCoords;
uniform sampler3D volume;
uniform sampler1D tf;
uniform float rayStep = 1/256.f;
in vec4 inPoint;
uniform float minStrength = 0.1;
out vec4 FragColor;

vec2 mapWindowToTexture()
{
	vec2 point;
	point = gl_FragCoord.xy / windowSize;
	return point;
}






void main()
{	
	vec3 outPoint = texture(bfCoords, mapWindowToTexture()).xyz;
	vec3 fullRay = (outPoint - inPoint.xyz);  
	float rayLength = length(fullRay);
	vec3 rayDir = normalize(fullRay);
	rayDir*= rayStep; // esto es para que el rayo esté escalado y sea todo más chola

	vec4 acumColor = vec4(0,0,0,1);
	
	vec3 ray = inPoint.xyz; 
	
	for(float travelDist = 0; travelDist <= rayLength; travelDist += rayStep, ray += rayDir)
	{
		vec4 colorSample = vec4(texture(volume, ray).rgb,1);
//		vec4 colorSample = texture(tf, texture(volume, ray).r);

		acumColor.rgb = (colorSample.rgb)* acumColor.a;
		
		acumColor.a*= 1- colorSample.a;
		
//		if(acumColor.a < minStrength)
//		{
//			break;
//		}
	}


	FragColor = acumColor;
//FragColor = vec4(vec3(rayLength),1);
//	FragColor = inPoint;
//	FragColor = vec4(outPoint,1);



	FragColor.a = 1;
}
