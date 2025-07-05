#version 330 core

out vec4 fragColor;

in vec3 vertex_texture;

uniform samplerCube cubemap;

void main()
{
    fragColor = texture(cubemap, vertex_texture);
}
