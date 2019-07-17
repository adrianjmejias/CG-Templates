#version 330 core

layout (location = 1) in vec2 o_uv;

out vec4 color;

uniform sampler2D albedo;

void main()
{
    color = texture(albedo, o_uv) * 0.8f;
}