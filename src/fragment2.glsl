#version 330 core

uniform float modulation;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0.f, modulation, 0.f, 0.5f);
}