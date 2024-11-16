#include "shaders.h"
#include "texture.h"
#include "box.h"
#include "clock.h"
#include "window.h"
#include "camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main()
{
    // Open window and initialize OpenGL
    bool init_success;
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, init_success);
    if (!init_success)
    {
        return -1;
    }

    // Build shaders program
    bool shader_success;
    Shaders program("shaders/vertex.glsl", "shaders/fragment.glsl", shader_success);
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

    // Load and activate texture units
    Texture tex1("resources/texture.jpg", false);
    Texture tex2("resources/texture2.png", true);
    program.use();
    program.uniform_texture("texture_img1", tex1);
    program.uniform_texture("texture_img2", tex2);

    // Create camera
    glm::vec3 camera_position(0.f, 0.f, -3.f);
    glm::vec3 camera_direction(0.f, 0.f, 1.f);
    glm::vec3 world_up(0.f, 1.f, 0.f);
    Camera camera(camera_position, camera_direction, world_up);
    const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    // Loop until the user closes the window
    Clock clock;
    while (window.next_frame_ready())
    {
        // Render boxes
        float delta_time = clock.tick();
        program.use();
        program.uniform_modulation("modulation");
        glm::mat4 view = camera.create_view(delta_time);
        glm::mat4 projection = camera.create_projection(aspect_ratio);
        for (int i = 0; i < num_boxes; i++)
        {
            glm::mat4 model = boxes[i].update(delta_time);
            glm::mat4 mv = view * model;
            glm::mat4 mvp = projection * view * model;
            program.uniform_mat4("mv", mv);
            program.uniform_mat4("mvp", mvp);
            boxes[i].draw();
        }
    }

    return 0;
}
