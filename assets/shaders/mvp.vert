#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_uv;
layout (location = 2) in vec3 iv_norm;

// Vertex data out data
out vec3 o_pos;
out vec2 o_uv;
out vec3 v_norm;

uniform mat4 transform;
uniform mat4 projection; //64  
uniform mat4 view;

uniform vec3 cameraPos;

uniform vec3 directionalPos;
uniform vec3 directionalDif;
uniform vec3 directionalSpec;

uniform vec3 pointPos;
uniform vec3 pointDif;
uniform vec3 pointSpec;


vec3 DirectionalContibution(){
	vec3 contribution = vec3(0,0,0);




 	return contribution;
}




void main()
{
    o_pos = vec3(transform * vec4(i_pos,1.0f));
    o_uv = i_uv;   
    v_norm =       iv_norm;

    vec4 worldPos = transform*vec4(i_pos,1.0f);







    gl_Position = projection* (view*worldPos);
}







