#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 m, mvp;
uniform mat3 m_for_normals;

out vec2 vertex_texture;
out vec3 vertex_normal; // in world space
out vec3 vertex_position; // in world space

void main()
{
    // Calculate vertex position in clip space
    gl_Position = mvp * vec4(position, 1.0);

    // Calculate data to be interpolated and used in fragment shader
    // For texturing
    vertex_texture = texcoord;
    // For lighting (in world space)
    vertex_normal = normalize(m_for_normals * normal);
    vertex_position = (m * vec4(position, 1.0)).xyz;
}