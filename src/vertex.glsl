#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

uniform float modulation;

out vec4 vertex_color;

void main()
{
    gl_Position = vec4(position.x, ((modulation*2) - 1) * position.y, 0.0, 1.0);
    vertex_color = vec4(color, 1);
}


