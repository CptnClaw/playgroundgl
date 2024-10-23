#version 330 core

uniform float modulation;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0.f, abs(2*modulation - 1), 0.f, 0.5f);
}