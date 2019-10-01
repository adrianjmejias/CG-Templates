#version 330 core


in vec2 TexCoords;

out vec4 FragColor;
in vec3 pos;
in float zIndex;
uniform sampler1D tf;

void main()
{            
vec4 color = texture(tf, TexCoords.x);
	FragColor = color;
//FragColor = vec4(TexCoords.xy,0,1);
}
