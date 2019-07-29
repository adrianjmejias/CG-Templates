#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform struct Material
{
	vec3 kD;
	vec3 kA;
	vec3 kS;
	vec3 kE;
	float IOR;
	float Ns;
} MAT;

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

uniform sampler2D tex_kD;
uniform sampler2D tex_Bump;
uniform sampler2D tex_displacement;

uniform float minLayers = 8;
uniform float maxLayers = 200;

uniform vec3 viewPos;
uniform float heightScale;
in struct {
	vec2 uv;
	vec3 pos;
	vec3 normal;
	vec3 TangentPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
} OBJ;


out vec4 FragColor;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(tex_displacement, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(tex_displacement, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }
    
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(tex_displacement, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    vec3 viewDir = normalize(OBJ.TangentViewPos - OBJ.TangentPos);
    vec3 lightDir = -normalize(OBJ.TangentLightPos - OBJ.TangentPos);

	vec2 uv = ParallaxMapping(OBJ.uv, viewDir);
    vec3 normal = texture(tex_Bump, uv).rgb;
    
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    vec3 color = texture(tex_kD, uv).rgb;
    vec3 ambient = MAT.kA * color;
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), MAT.Ns);

    vec3 specular = MAT.kS * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);

//FragColor = texture(tex_displacement, OBJ.uv);













//	colorsito = texture(tex_kD, OBJ.uv.xy);

//	vec3 normal = texture(tex_Bump ,OBJ.uv.xy).xyz;
//	normal = (normal *2.f) -1;
//
//	colorsito = LigthCalc(normal);
//uniform sampler2D tex_kD;
//uniform sampler2D tex_Bump;
//uniform sampler2D tex_d;
//	colorsito = texture(tex_displacement , Parallax(OBJ.uv.xy, iTBN*normalize((viewPos - OBJ.pos.xyz))));

//colorsito = vec4(gl_FragCoord.zzz,1);
}
