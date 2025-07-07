#version 330 core

in vec3 vertex_normal; // in world space
in vec3 vertex_position; // in world space

uniform samplerCube cubemap;
uniform vec3 camera_position; // in world space

out vec4 fragColor;

void main()
{
    vec3 eye_direction = normalize(vertex_position - camera_position);
    vec3 reflected_direction = reflect(eye_direction, vertex_normal);
    fragColor = vec4(texture(cubemap, reflected_direction).rgb, 1.0);
}
