#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 mv, mvp;
uniform mat4 transform;

out vec2 vertex_texture;

void main()
{
    gl_Position = mvp * vec4(position.xyz, 1.0);
    vertex_texture = texcoord;
}


