#include <iostream>
#include <memory>

#include <glad/gl.h>

#include "model.h"
#include "shaders.h"
#include "texture.h"
#include "clock.h"
#include "window.h"
#include "camera.h"
#include "lightsource.h"
#include "flashlight.h"
#include "sun.h"
#include "ground.h"
#include "selection.h"  

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define PI 3.14159f

using Scene = std::vector<std::unique_ptr<Model>>;

// From callbacks.cpp
extern bool mode_selection; 
extern bool mouse_clicked;
extern uint click_x, click_y;

void populate_scene(Scene &models)
{
    models.emplace_back(std::make_unique<Model>("resources/backpack/backpack.obj"));
    models.back()->translate(-3.f, -.3f, 1.f);
    models.back()->rotate(PI * 7.f/10.f, 0.f, 1.f, 0.f);
    models.back()->scale(0.4);

    models.emplace_back(std::make_unique<Model>("resources/cbox/cbox.obj"));
    models.back()->translate(0.f, -.5f, 1.f);
    models.back()->scale(0.5);
    models.emplace_back(std::make_unique<Model>("resources/cbox/cbox.obj"));
    models.back()->translate(1.f, -.5f, 1.f);
    models.back()->scale(0.5);
    models.emplace_back(std::make_unique<Model>("resources/cbox/cbox.obj"));
    models.back()->translate(1.f, .5f, 1.f);
    models.back()->scale(0.5);
    models.emplace_back(std::make_unique<Model>("resources/cbox/cbox.obj"));
    models.back()->translate(1.f, -.5f, 0.f);
    models.back()->scale(0.5);
    
    models.emplace_back(std::make_unique<Model>("resources/playground/KIDS_PLAYGROUND.obj"));
    models.back()->translate(4.f, -1.f, 7.f);
}

int main()
{
    // Open window and initialize OpenGL
    bool init_success;
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, init_success);
    if (!init_success)  return -1;

    // Build shaders programs
    bool shader_success;
    Shaders program("shaders/vertex.glsl", "shaders/fragment.glsl", shader_success);
    if (!shader_success)  return -1; 
    Shaders program_light("shaders/vertex.glsl", "shaders/fragment_light.glsl", shader_success);
    if (!shader_success)  return -1;
    Shaders program_object_id("shaders/vertex.glsl", "shaders/fragment_objectid.glsl", shader_success);
    if (!shader_success)  return -1;

    // Create camera
    glm::vec3 camera_position(0.f, 0.f, -10.f);
    glm::vec3 camera_direction(0.f, 0.f, 1.f);
    glm::vec3 world_up(0.f, 1.f, 0.f);
    const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    Camera camera(camera_position, camera_direction, world_up, aspect_ratio);

    // Construct light sources to render
    // Point light
    LightSource lightsource(glm::vec3(6.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    program_light.use();
    program_light.uniform_vec3("color", lightsource.color);
    program.use();
    program.uniform_float("ambient_light_intensity", .2f);
    // Sunlight
    Sun sun(glm::vec3(0.f, -1.f, 0.f));
    // Flashlight
    Flashlight flashlight(-3.f*PI/25.f, 3.f*PI/25.f, -2.f*PI/3.f, -PI/3.f);
    
    // Load scenes
    Scene scene;
    populate_scene(scene);
    Ground ground(-1.f, 100,
        std::make_unique<Texture>("resources/ground.jpg", TextureType::Diffuse), 
        std::make_unique<Texture>("resources/blank.png", TextureType::Specular));
    
    // Prepare object selection mechanism
    Selection selection(WINDOW_WIDTH, WINDOW_HEIGHT, init_success);
    if (!init_success)  return -1;
    std::vector<bool> is_selected(scene.size(), false);

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
        program_light.uniform_mat4("mvp", proj_matrix * view_matrix * lightsource.model);
        lightsource.draw();

        // Setup lighting of all objects
        program.use();
        lightsource.use(program, view_matrix);
        sun.use(program, view_matrix);
        flashlight.use(program);
        
        // Draw scene (non-selected objects only)
        ground.draw(program, view_matrix, proj_matrix);
        uint object_id = 0;
        for (const std::unique_ptr<Model> &model : scene)
        {
            if (!is_selected[object_id++])
            {
                model->draw(program, view_matrix, proj_matrix);
            }
        }

        // Draw scene (selected objects only, always on top)
        object_id = 0;
        for (const std::unique_ptr<Model> &model : scene)
        {
            if (is_selected[object_id++]) 
            {
                model->draw_with_outline(program, program_light, view_matrix, proj_matrix);
            }
        }
                    
        // Second render pass off-screen for object selection
        if (mode_selection)
        {
            selection.start();
            program_object_id.use();
            uint object_id = 1; 
            for (const std::unique_ptr<Model> &model : scene)
            {
                program_object_id.uniform_uint("object_id", object_id++);
                model->draw_simple(program_object_id, proj_matrix * view_matrix * model->world_transform);
            }
            if (mouse_clicked)
            {
                uint selected_object_id = selection.object_at(click_x, WINDOW_HEIGHT - click_y);
                if (selected_object_id > 0)
                {
                    is_selected[selected_object_id - 1] = !is_selected[selected_object_id - 1];
                    std::cout << "Object at (" << click_x << "," << click_y << ") ";
                    std::cout << "is " << selection.object_at(click_x, WINDOW_HEIGHT - click_y) << std::endl;
                }
                mouse_clicked = false;
            }
            selection.end();
        }
    }

    return 0;
}
