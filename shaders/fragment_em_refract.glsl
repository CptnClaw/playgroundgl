#version 330 core

in vec3 vertex_normal; // in world space
in vec3 vertex_position; // in world space

uniform samplerCube cubemap;
uniform vec3 camera_position; // in world space

out vec4 fragColor;

void main()
{
    float ior = 1.0 / 1.52; // Approximately air to glass index of refraction ratio
    vec3 camera_direction = normalize(vertex_position - camera_position);
    vec3 refracted_direction = refract(camera_direction, vertex_normal, ior);
    fragColor = vec4(texture(cubemap, refracted_direction).rgb, 1.0);
}
