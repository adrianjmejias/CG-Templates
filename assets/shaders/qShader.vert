#version 330 core
layout (location = 0) in vec3 gPosition;
out vec2 uv;

void main()
{    
    uv = (gPosition.xy+1)/2.f;
    gl_Position = vec4(gPosition, 1.0);
}