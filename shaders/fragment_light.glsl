#version 330 core

uniform vec3 light_color;
out vec4 fragColor;

void main()
{
    fragColor = vec4(light_color, 1.0);
    fragColor = vec4(1.0);
}