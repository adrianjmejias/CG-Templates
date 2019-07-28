

#version 330 core
out vec4 Fragvec3;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube skybox;

uniform float IOR;
uniform struct{
	float IOR;
} MAT;


void main()
{             
    float ratio =  IOR/ MAT.IOR;
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    Fragvec3 = vec4(texture(skybox, R).rgb, 1.0);
}