#include <iostream>
#include <memory>

#include <glad/gl.h>

#include "model.h"
#include "shaders.h"
#include "texture.h"
#include "box.h"
#include "clock.h"
#include "window.h"
#include "camera.h"
#include "lightsource.h"
#include "flashlight.h"
#include "sun.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define PI 3.14159f

using Scene = std::vector<std::unique_ptr<Model>>;


void populate_scene(Scene &models)
{
    models.emplace_back(std::make_unique<Model>("resources/kokorecci/kokorecci.obj"));
    models.back()->translate(5.f, -1.f, -1.f);
    models.back()->rotate(PI, 0.f, 1.f, 0.f);

    models.emplace_back(std::make_unique<Model>("resources/backpack/backpack.obj"));
    models.back()->translate(-5.f, 0.f, 0.f);
    models.back()->rotate(PI, 0.f, 1.f, 0.f);

    models.emplace_back(std::make_unique<Model>("resources/cbox/cbox.obj"));
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

    // Create camera
    glm::vec3 camera_position(0.f, 0.f, -10.f);
    glm::vec3 camera_direction(0.f, 0.f, 1.f);
    glm::vec3 world_up(0.f, 1.f, 0.f);
    const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    Camera camera(camera_position, camera_direction, world_up, aspect_ratio);

    // Construct light sources to render
    // Point light
    LightSource lightsource(glm::vec3(0.f, 2.f, -4.f), glm::vec3(1.f, 1.f, 1.f));
    program_light.use();
    program_light.uniform_vec3("color", lightsource.get_color());
    program.use();
    program.uniform_float("ambient_light_intensity", .1f);
    // Sunlight
    Sun sun(glm::vec3(0.f, -1.f, 0.f));
    // Flashlight
    Flashlight flashlight(-3.f*PI/25.f, 3.f*PI/25.f, -2.f*PI/3.f, -PI/3.f);
    
    // Load scenes
    Scene scene;
    populate_scene(scene);

    // Loop until the user closes the window
    Clock clock;
    while (window.next_frame_ready())
    {
        // Keep time since last frame and update camera
        float delta_time = clock.tick();
        camera.update(delta_time);
        const glm::mat4 &view_matrix = camera.get_view();
        const glm::mat4 &proj_matrix = camera.get_projection();
        
        // Render light source (emissive small box)
        program_light.use();
        lightsource.update(delta_time);
        program_light.uniform_mat4("mvp", proj_matrix * view_matrix * lightsource.get_model());
        lightsource.draw();

        // Setup lighting of all objects
        program.use();
        lightsource.use(program, view_matrix);
        sun.use(program, view_matrix);
        flashlight.use(program);
        
        // Draw scene
        for (std::unique_ptr<Model> &model : scene)
        {
            // model->spin(delta_time);
            model->draw(program, view_matrix, proj_matrix);
        }
    }

    return 0;
}
