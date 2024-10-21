#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texcoord;

uniform float modulation;
uniform vec2 movement;
uniform float zoom;

out vec4 vertex_color;
out vec2 vertex_texture;

void main()
{
    gl_Position = vec4(position.x, ((modulation*2) - 1) * position.y, 0.0, 1.0);
    vertex_color = vec4(color, 1);
    vertex_texture = 2 * zoom * (texcoord + movement);
}


