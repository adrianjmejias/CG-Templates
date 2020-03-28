#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gBloom;
layout (location = 3) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


uniform sampler2D texture_diffuse;
// uniform sampler2D texture_specular1;

uniform vec3 kD;
uniform vec3 kS;
out vec4 fragColor;
uniform int bloom = 0;
uniform float bloomThreshold = 0.5f;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal.rgb = normalize(Normal);
    gNormal.a = 1 - bloom;

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb =texture(texture_diffuse, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 0; //texture(texture_specular1, TexCoords).r;


    float brightness = dot(gAlbedoSpec.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(bloomThreshold < brightness)
    {
        gBloom.rgb = gAlbedoSpec.rgb;
    }
    // gPosition = gAlbedoSpec.rgb;
    // gPosition = gAlbedoSpec.rgb;  
    // gPosition = gAlbedoSpec.rgb;
}