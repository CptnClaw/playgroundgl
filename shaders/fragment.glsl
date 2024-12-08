#version 330 core

struct Material
{
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};
struct Light
{
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
    vec3 color;

    vec4 position; // Point light
    vec4 sun_direction;  // Sunlight
    vec4 flashlight_direction; // Flashlight
    
    float active_range; // used for calculating attenuation of point light
};

in vec2 vertex_texture;
in vec3 vertex_normal; // in view space
in vec3 vertex_position; // in view space

uniform Light light;
uniform Material material;

out vec4 fragColor;

void main()
{
    // Read textures
    vec4 diffuse_color = texture(material.diffuse_map, vertex_texture);
    vec4 specular_color = texture(material.specular_map, vertex_texture);
    
    // Ambient term
    vec3 ambient = vec3(light.ambient_intensity);
    
    // Sunlight
    vec3 sun = normalize(- light.sun_direction.xyz) * (1.0 - light.sun_direction.w); // directional light
    
    // Flashlight
    vec3 camera_direction = normalize(-vertex_position); // Camera is in origin (view space)
    vec3 flashlight_direction = normalize(-light.flashlight_direction.xyz);
    float flashlight_on = light.flashlight_direction.w;
    float flashlight_angle = dot(camera_direction, flashlight_direction);
    float flashlight_intensity = pow(clamp(flashlight_angle + 0.01, 0.0, 1.0), 100);
    vec3 flashlight_color = vec3(1.0) * flashlight_intensity * flashlight_on;

    // Diffuse term
    vec3 fragment_to_light = light.position.xyz - vertex_position;
    vec3 light_direction = normalize(fragment_to_light) * (light.position.w); // point light
    float diffuse_geometric_term = max(0, dot(light_direction, vertex_normal));
    vec3 diffuse = vec3(light.diffuse_intensity * diffuse_geometric_term);

    // Specular term
    vec3 reflected_light_direction = reflect(-light_direction, vertex_normal);
    float specular_geometric_term = max(0, dot(camera_direction, reflected_light_direction));
    specular_geometric_term = pow(specular_geometric_term, material.shininess * 128.0);
    vec3 specular = (light.specular_intensity * specular_geometric_term) * light.color;
    
    // Attenuation
    float d = length(fragment_to_light);
    d = d * light.sun_direction.w; // make d=0, attenuation=1 for directional light
    float linear_term = mix(0.22, 0.027, light.active_range);
    float quadratic_term = mix(0.2, 0.0028, light.active_range);
    float attenuation = 1.0 / (1 + linear_term * d + quadratic_term * d * d);
    diffuse *= attenuation;
    specular *= attenuation;

    // All together
    fragColor = vec4(ambient + diffuse + flashlight_color, 1.0) * diffuse_color + vec4(specular, 1.0) * specular_color;
}
