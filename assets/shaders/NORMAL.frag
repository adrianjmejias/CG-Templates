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

vec2 Parallax(vec2 uv, vec3 viewDir)
{
	float height = texture(tex_displacement, uv).r;
	vec2 p = viewDir.xy/viewDir.z * (height*heightScale);
	return uv -p;
}

void main()
{

  // obtain normal from normal map in range [0,1]
    vec3 normal = texture(tex_Bump, OBJ.uv).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(tex_kD, OBJ.uv).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = -normalize(OBJ.TangentLightPos - OBJ.TangentPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(OBJ.TangentViewPos - OBJ.TangentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
//FragColor = texture(tex_displacement, OBJ.uv)















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
