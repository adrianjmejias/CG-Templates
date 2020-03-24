#version 330 core
out vec4 Fragvec3;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    Fragvec3 = texture(skybox, TexCoords);
}