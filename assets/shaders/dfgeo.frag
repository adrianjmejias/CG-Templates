#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_specular1;

uniform vec3 kD;
uniform vec3 kS;
out vec4 fragColor;
uniform int bloom = 0;


void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal.rgb = normalize(Normal);

    gNormal.a = 1 - bloom;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = kD; //texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 0; //texture(texture_specular1, TexCoords).r;


    // gPosition.rgb = k;
    // gPosition = gAlbedoSpec.rgb;
}