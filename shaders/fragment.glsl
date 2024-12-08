#version 330 core
struct Material
{
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};
struct LightSource
{
    float is_on;
    float diffuse_intensity;
    float specular_intensity;
    vec3 color;
    vec3 position; 
    vec3 direction;
    float strength; // used for calculating attenuation
};

in vec2 vertex_texture;
in vec3 vertex_normal; // in view space
in vec3 vertex_position; // in view space

uniform float ambient_light_intensity; 
uniform LightSource light;
uniform LightSource sun;
uniform LightSource flashlight;
uniform Material material;

out vec4 fragColor;


vec3 CalcDiffuse(vec3 light_direction, vec3 target_normal)
{
    return vec3(max(0, dot(light_direction, target_normal)));
}

vec3 CalcSpecular(vec3 light_direction, vec3 target, vec3 target_normal)
{
    vec3 reflected_light_direction = reflect(-light_direction, target_normal);
    vec3 camera_direction = normalize(-target); // Camera is in origin (view space)
    float specular_geometric_term = max(0, dot(camera_direction, reflected_light_direction));
    return vec3(pow(specular_geometric_term, material.shininess * 128.0));
}

float CalcAttenuation(float distance, float strength)
{
    // Attenuation decreases quadratically as a function of distance
    float linear_term = mix(0.22, 0.027, strength);
    float quadratic_term = mix(0.2, 0.0028, strength);
    return 1.0 / (1 + linear_term * distance + quadratic_term * distance * distance);
}

vec3 CalcPointLight(LightSource source, vec3 target, vec3 target_normal, vec3 diffuse_color, vec3 specular_color)
{
    // Calculate diffuse and specular components
    vec3 fragment_to_light = source.position - target;
    vec3 light_direction = normalize(fragment_to_light);
    vec3 diffuse = source.diffuse_intensity * CalcDiffuse(light_direction, target_normal);
    vec3 specular = source.specular_intensity * CalcSpecular(light_direction, target, target_normal);
    
    // Attenuate components using distance from fragment to light source
    float attenuation = CalcAttenuation(length(fragment_to_light), source.strength);
    diffuse *= attenuation;
    specular *= attenuation;
    
    // Return combined light
    return diffuse * diffuse_color + specular * specular_color;
}

vec3 CalcFlashlight(LightSource source, vec3 target, vec3 diffuse_color)
{
    // Calculate light in a small disk in front of camera
    // Light is flat, without specular component
    vec3 camera_direction = normalize(-target); // Camera is in origin (view space)
    vec3 flashlight_direction = normalize(-source.direction);
    float cosine_similarity = dot(camera_direction, flashlight_direction);
    float flashlight_intensity = pow(clamp(cosine_similarity + 0.01, 0.0, 1.0), 100); // Light up a disk with fast decaying surrounding

    // Attenuate light using distance from fragment to camera
    float attenuation = CalcAttenuation(length(target), source.strength);
    return source.color * diffuse_color * attenuation * flashlight_intensity * source.is_on;
}

vec3 CalcSun(LightSource source, vec3 target, vec3 target_normal, vec3 diffuse_color, vec3 specular_color)
{
    // Calculate light coming from infinity in a prescribed direction (rays are parallel)
    vec3 light_direction = normalize(-source.direction);
    vec3 diffuse = source.diffuse_intensity * CalcDiffuse(light_direction, target_normal);
    vec3 specular = source.specular_intensity * CalcSpecular(light_direction, target, target_normal);
    return source.is_on * source.color * (diffuse * diffuse_color + specular * specular_color);
}

void main()
{
    // Read textures
    vec3 diffuse_color = texture(material.diffuse_map, vertex_texture).rgb;
    vec3 specular_color = texture(material.specular_map, vertex_texture).rgb;

    // Ambient light
    vec3 final_color = vec3(ambient_light_intensity) * diffuse_color;
    
    // Directed lights
    final_color += CalcSun(sun, vertex_position, vertex_normal, diffuse_color, specular_color);
    final_color += CalcFlashlight(flashlight, vertex_position, diffuse_color);
    
    // Point lights
    final_color += CalcPointLight(light, vertex_position, vertex_normal, diffuse_color, specular_color);

    // All together
    fragColor = vec4(final_color, 1.0);
}
