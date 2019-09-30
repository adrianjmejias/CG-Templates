#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler3D volume;
uniform mat4 projection;
uniform mat4 view;
uniform float deltaTime;
uniform int axis;




void main()
{            

	//FragColor = vec4(texCoords,0,1);
	FragColor = texture(volume, vec3(deltaTime,texCoords.x, texCoords.y));
//	discard;
}