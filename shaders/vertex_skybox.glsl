#version 330 core

layout (location = 0) in vec3 position;

// projection * view transformations (model = I)
uniform mat4 vp; 

out vec3 vertex_texture;

void main()
{
    // In a skybox, texture (uv) coord = xyz coord
    vertex_texture = position;

    // Camera transform
    vec4 new_position = vp * vec4(position, 1.0);
    
    // Normalize z coordiate to the far plane, so that the skybox will appear behind all other objects
    // This necessitates changeing the depth function to LEQUAL so because otherwise the far plane is dropped by the default depth test
    gl_Position  = new_position.xyww;
}


