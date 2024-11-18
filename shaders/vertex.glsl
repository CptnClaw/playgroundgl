#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 model, mvp;

out vec2 vertex_texture;
out vec3 vertex_normal; // in world coordinates
out vec3 vertex_position; // in world coordinates

void main()
{
    gl_Position = mvp * vec4(position.xyz, 1.0);
    vertex_texture = texcoord;
    // vertex_normal = normalize((model * vec4(normal, 0.0)).xyz);
    vertex_normal = mat3(transpose(inverse(model))) * normal;
    vertex_position = (model * vec4(position, 1.0)).xyz;
}


