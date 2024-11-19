#version 330 core

in vec2 vertex_texture;
in vec3 vertex_normal; // in view space
in vec3 vertex_position; // in view space

uniform vec3 light_position; // in view space
uniform vec3 light_color;

uniform sampler2D texture_img1;
uniform sampler2D texture_img2;
uniform float modulation;

out vec4 fragColor;

void main()
{
    // Calculate fragment texture
    vec4 color1 = texture(texture_img1, vertex_texture);
    vec4 color2 = texture(texture_img2, vec2(1 - vertex_texture.x, 1 - vertex_texture.y));
    float blend = pow(1 - modulation, 2);
    vec4 texColor = mix(color1, color2, blend);
    
    // Ambient term
    vec4 ambient = 0.1 * vec4(1.0, 1.0, 1.0, 1.0); // Ambient white light
    
    // Diffuse term
    vec3 light_direction = normalize(light_position - vertex_position);
    float diffuse_intensity = max(0, dot(light_direction, vertex_normal));
    vec4 diffuse = 0.9 * diffuse_intensity * vec4(light_color, 1.0);

    // Specular term
    vec3 camera_direction = normalize(-vertex_position); // Camera is in origin (view space)
    vec3 reflected_light_direction = reflect(-light_direction, vertex_normal);
    float specular_intensity = max(0, dot(camera_direction, reflected_light_direction));
    specular_intensity = pow(specular_intensity, 64);
    vec4 specular = 0.5 * specular_intensity * vec4(light_color, 1.0);

    // All together
    fragColor = (ambient + diffuse) * texColor + specular;
}
