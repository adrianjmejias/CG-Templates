

#version 330 core
out vec4 Fragvec3;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{             
    float ratio = 1.3 / 1.f;
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    Fragvec3 = vec4(texture(skybox, R).rgb, 1.0);
}