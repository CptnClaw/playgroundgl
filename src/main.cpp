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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159f



void render(const Shaders &prog, const LightSource &lt, const Camera &camera)
{
    prog.use();
    glm::mat4 mvp = camera.get_projection() * camera.get_view() * lt.get_model();
    prog.uniform_mat4("mvp", mvp);
    lt.draw();
}


class BoxesScene
{
public:
    BoxesScene() :
        tex1("resources/crate2.png", true, TextureType::Diffuse),
        tex2("resources/crate2_specular.png", true, TextureType::Specular)
    {
        // Construct boxes to render
        float rotation = 0.f;
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(0.0f, 0.0f, 0.0f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(5.0f, 5.0f, 3.0f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(-1.5f, -2.2f,-2.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(-3.8f, -2.0f, -.3f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(2.4f, -0.4f, -3.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(-1.f, 3.0f, 7.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(1.3f, -2.0f, -2.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(1.5f, 2.0f, -2.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(1.5f, 0.2f, -1.5f), rotation++));
        boxes.emplace_back(std::make_unique<Box>(glm::vec3(-1.3f, 1.0f, -1.5f), rotation++));
    }
    
    void draw(float delta_time, const Shaders &program, const Camera &camera)
    {
        // Apply textures
        program.use();
        tex1.activate(0);
        program.uniform_int("material.diffuse_map1", 0);
        tex2.activate(1);
        program.uniform_int("material.specular_map1", 1);
        program.uniform_float("material.shininess", .5f);

        // Render boxes
        for (const std::unique_ptr<Box> &b : boxes) 
        {
            b->update(delta_time);
            render(program, b, camera);
        }
    }
    
private:
    std::vector<std::unique_ptr<Box>> boxes;
    Texture tex1, tex2;

    static void render(const Shaders &prog, const std::unique_ptr<Box> &box, const Camera &camera)
    {
        prog.use();
        glm::mat4 mv = camera.get_view() * box->get_model();
        glm::mat4 mvp = camera.get_projection() * mv;
        prog.uniform_mat4("mv", mv);
        prog.uniform_mat4("mvp", mvp);
        prog.uniform_mat3("mv_for_normals", glm::transpose(glm::inverse(mv)));
        box->draw();
    }
};

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
    BoxesScene scene;
    Model backpack_scene("resources/backpack.obj");

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
        render(program_light, lightsource, camera);

        // Setup lighting
        program.use();
        lightsource.use(program, camera.get_view());
        sun.use(program, camera.get_view());
        flashlight.use(program);
        
        // Draw scenes
        backpack_scene.draw(program, camera.get_view(), camera.get_projection());
        scene.draw(delta_time, program, camera);
    }

    return 0;
}
