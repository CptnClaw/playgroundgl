#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 mv, mvp;
uniform mat3 mv_for_normals;

out vec2 vertex_texture;
out vec3 vertex_normal; // in view space
out vec3 vertex_position; // in view space

void main()
{
    // Calculate vertex position in clip space
    gl_Position = mvp * vec4(position.xyz, 1.0);

    // Calculate data to be interpolated and used in fragment shader
    // Texture 
    vertex_texture = texcoord;
    // Lighting (in view space)
    vertex_normal = mv_for_normals * normal;
    vertex_position = (mv * vec4(position, 1.0)).xyz;
}


