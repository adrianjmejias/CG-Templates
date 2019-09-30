#version 330 core


in vec4 TexCoords;

out vec4 FragColor;
in float zIndex;


void main()
{            
	FragColor = vec4(zIndex,0,0,1);
}
