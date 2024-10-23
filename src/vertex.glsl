#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

uniform mat4 mv, mvp;
uniform mat4 transform;

out vec4 vertex_color;
out vec2 vertex_texture;

void main()
{
    gl_Position = mvp * vec4(position.xy, 0.0, 1.0);
    vertex_color = vec4(color, 1);
    vertex_texture = texcoord;
}


