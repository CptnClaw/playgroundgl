#include "shaders.h"
#include "texture.h"
#include "box.h"
#include "clock.h"
#include "window.h"
#include "camera.h"
#include "lightsource.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


void render(const Shaders &prog, const Box &box, const Camera &camera)
{
    glm::mat4 mv = camera.get_view() * box.get_model();
    glm::mat4 mvp = camera.get_projection() * mv;
    prog.uniform_mat4("mv", mv);
    prog.uniform_mat4("mvp", mvp);
    prog.uniform_mat3("mv_for_normals", glm::transpose(glm::inverse(mv)));
    box.draw();
}

int main()
{
    // Open window and initialize OpenGL
    bool init_success;
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, init_success);
    if (!init_success)
    {
        return -1;
    }

    // Build shaders programs
    bool shader_success;
    Shaders program("shaders/vertex.glsl", "shaders/fragment.glsl", shader_success);
    if (!shader_success)
    {
        return -1;
    }
    Shaders program_light("shaders/vertex.glsl", "shaders/fragment_light.glsl", shader_success);
    if (!shader_success)
    {
        return -1;
    }

    // Construct boxes to render
    float rotation = 0.f;
    Box boxes[] = {
        Box(glm::vec3(0.0f, 0.0f, 0.0f), rotation++),
        Box(glm::vec3(2.0f, 5.0f, -15.0f), rotation++),
        Box(glm::vec3(-1.5f, -2.2f, -2.5f), rotation++),
        Box(glm::vec3(-3.8f, -2.0f, -12.3f), rotation++),
        Box(glm::vec3(2.4f, -0.4f, -3.5f), rotation++),
        Box(glm::vec3(-1.7f, 3.0f, -7.5f), rotation++),
        Box(glm::vec3(1.3f, -2.0f, -2.5f), rotation++),
        Box(glm::vec3(1.5f, 2.0f, -2.5f), rotation++),
        Box(glm::vec3(1.5f, 0.2f, -1.5f), rotation++),
        Box(glm::vec3(-1.3f, 1.0f, -1.5f), rotation++)
    };
    int num_boxes = sizeof(boxes) / sizeof(Box);
    
    // Construct light source to render
    LightSource lightsource(glm::vec3(0.f, 2.f, -4.f), glm::vec3(1.f, 1.f, 1.f));

    // Load and activate texture units
    Texture tex1("resources/crate2.png", true);
    Texture tex2("resources/crate2_specular.png", true);
    program.use();
    program.uniform_texture("material.diffuse_map", tex1);
    program.uniform_texture("material.specular_map", tex2);

    // Create camera
    glm::vec3 camera_position(0.f, 0.f, -10.f);
    glm::vec3 camera_direction(0.f, 0.f, 1.f);
    glm::vec3 world_up(0.f, 1.f, 0.f);
    const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    Camera camera(camera_position, camera_direction, world_up, aspect_ratio);

    // Loop until the user closes the window
    Clock clock;
    while (window.next_frame_ready())
    {
        // Keep time since last frame and update camera
        float delta_time = clock.tick();
        camera.update(delta_time);
        
        // Render light source
        program_light.use();
        lightsource.update(delta_time);
        program_light.uniform_vec3("light_color", lightsource.get_color());
        render(program_light, lightsource, camera);

        // Render boxes
        program.use();
        program.uniform_float("light.ambient_intensity", 0.2);
        program.uniform_float("light.diffuse_intensity", 0.8);
        program.uniform_float("light.specular_intensity", 0.5);
        program.uniform_vec3("light.position", lightsource.get_position(camera.get_view()));
        program.uniform_float("material.shininess", .5f);
        for (int i = 0; i < num_boxes; i++)
        {
            boxes[i].update(delta_time);
            render(program, boxes[i], camera);
        }
    }

    return 0;
}
