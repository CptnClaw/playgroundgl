#version 330 core

in vec2 vertex_texture;

uniform sampler2D texture_img1;
uniform sampler2D texture_img2;
uniform float modulation;

out vec4 fragColor;

void main()
{
    vec4 color1 = texture(texture_img1, vertex_texture);
    vec4 color2 = texture(texture_img2, vec2(1 - vertex_texture.x, 1 - vertex_texture.y));
    float blend = pow(1 - modulation, 2);
    fragColor = mix(color1, color2, blend);
}