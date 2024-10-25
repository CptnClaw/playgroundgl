#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "callbacks.h"
#include "shaders.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159f
#define EPSILON .001f
#define ALMOST_PI (PI-EPSILON)
#define MOVE_SPEED 2.f

extern uint shader_program;
extern int move_x, move_y;
extern float rot_speed;
extern float pitch, yaw;
extern float zoom;

float boxes_rotation = 0;
glm::vec3 camera_position(0.f, 0.f, -3.f);
glm::vec3 camera_direction(0.f, 0.f, 1.f);
glm::vec3 world_up(0.f, 1.f, 0.f);

glm::mat4 create_model(glm::vec3 pos, float rot)
{
    glm::mat4 matrix(1.f); // Unit matrix
    matrix = glm::translate(matrix, pos); 
    return glm::rotate(matrix,  rot * -PI / 3, glm::vec3(1.f, 2.f, 0.f));
}

glm::mat4 create_view(float delta_time)
{
    if (pitch > ALMOST_PI / 2) pitch = ALMOST_PI / 2;
    else if (pitch < -(ALMOST_PI) / 2) pitch = -(ALMOST_PI) / 2;
    camera_direction.x = cos(yaw) * cos(pitch);
    camera_direction.y = sin(pitch);
    camera_direction.z = sin(yaw) * cos(pitch);
    camera_direction = glm::normalize(camera_direction);
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_direction, world_up));
    camera_position = camera_position + 
                    camera_direction * (MOVE_SPEED * delta_time * move_y) + 
                    camera_right * (MOVE_SPEED * delta_time * move_x);
    return glm::lookAt(camera_position, camera_position + camera_direction, world_up);
}

glm::mat4 create_projection()
{
    // return glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 20.f);
    if (zoom > 1.f) zoom = 1.f;
    if (zoom < .2f) zoom = .2f;
    float fov = zoom * (PI / 4);
    return glm::perspective(fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, .1f, 100.f);
}

void update(uint program, glm::vec3 pos, int box_id, double delta_time)
{
    boxes_rotation += delta_time * rot_speed;
    glm::mat4 model = create_model(pos, box_id + boxes_rotation);
    glm::mat4 view = create_view(delta_time);
    glm::mat4 projection = create_projection();
    glm::mat4 mv = view * model;
    glm::mat4 mvp = projection * mv;
    send_matrix(program, "mv", mv);
    send_matrix(program, "mvp", mvp);
}

uint load_texture(std::string texture_path, GLenum format)
{
    // Prepare OpenGL texture
    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Read image file
    int texwidth, texheight, texchannels;
    unsigned char *texdata = stbi_load(texture_path.c_str(), &texwidth, &texheight, &texchannels, 0);

    // Copy image into GPU
    if (texdata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, format, GL_UNSIGNED_BYTE, texdata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    stbi_image_free(texdata);
    return texture;
}


int main()
{
    // Initialize the library 
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "playgroundgl", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to open a window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, fb_sz_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Compile shaders
    shader_program = build_program("src/vertex.glsl", "src/fragment.glsl");
    if (0 == shader_program)
    {
        glfwTerminate();
        return -1;
    }

    // Define triangles
    float vertices[] = {
        // 2D position    // texture coords
        -.5f, -.5f, -.5f,      0.f, 0.f,      // bottom left
         .5f, -.5f, -.5f,      1.f, 0.f,      // bottom right
        -.5f,  .5f, -.5f,      0.f, 1.f,      // top left
         .5f,  .5f, -.5f,      1.f, 1.f,      // top right
                                              //
        -.5f, -.5f, .5f,       0.f, 0.f,      // 
         .5f, -.5f, .5f,       1.f, 0.f,      // 
        -.5f,  .5f, .5f,       0.f, 1.f,      // 
         .5f,  .5f, .5f,       1.f, 1.f,      // 
                                              // 
        -.5f,  .5f, .5f,       0.f, 0.f,      // 
         .5f,  .5f, .5f,       1.f, 0.f,      // 
        -.5f,  .5f, -.5f,      0.f, 1.f,      // 
         .5f,  .5f, -.5f,      1.f, 1.f,      // 
                                              //
        -.5f, -.5f, .5f,       0.f, 0.f,      // 
         .5f, -.5f, .5f,       1.f, 0.f,      // 
        -.5f, -.5f, -.5f,      0.f, 1.f,      // 
         .5f, -.5f, -.5f,      1.f, 1.f,      // 
                                              //
         .5f,  .5f,  .5f,      0.f, 0.f,      // 
         .5f, -.5f,  .5f,      1.f, 0.f,      // 
         .5f,  .5f, -.5f,      0.f, 1.f,      // 
         .5f, -.5f, -.5f,      1.f, 1.f,      // 
                                              //
         -.5f,  .5f, .5f,      0.f, 0.f,      // 
         -.5f, -.5f, .5f,      1.f, 0.f,      // 
         -.5f,  .5f,-.5f,      0.f, 1.f,      // 
         -.5f, -.5f,-.5f,      1.f, 1.f       // 
                                               
    };
    uint indices[] = {
        0, 1, 2, 1, 2, 3, // back
        4, 5, 6, 5, 6, 7, // front
        8, 9,10, 9,10,11, // top
        12,13,14,13,14,15,// bottom
        16,17,18,17,18,19,// right
        20,21,22,21,22,23 // left
    };
    int num_vertices = sizeof(indices) / sizeof(uint);

    glm::vec3 boxes[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    int num_boxes = sizeof(boxes) / sizeof(glm::vec3);

    // Send vertices to GPU
    uint vbuf;
    uint ibuf;
    uint array_obj;
    glGenBuffers(1, &vbuf);
    glGenBuffers(1, &ibuf);
    glGenVertexArrays(1, &array_obj);

    glBindVertexArray(array_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    size_t stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures
    uint textures[] = {
        load_texture("resources/texture.jpg", GL_RGB),
        load_texture("resources/texture2.png", GL_RGBA)
    };
    
    // Activate texture units 
    for (int i=0; i<2; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    
    // Connect texture units with samplers in fragment shaders
    glUseProgram(shader_program);
    int samplers[] = {
        glGetUniformLocation(shader_program, "texture_img1"),
        glGetUniformLocation(shader_program, "texture_img2")};
    for (int j = 0; j < 2; j++)
    {
        glUniform1i(samplers[j], j);
    }

    // Loop until the user closes the window
    double time_before = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Run program
        double time_now = glfwGetTime();
        double delta_time = time_now - time_before;
        time_before = time_now;
        glUseProgram(shader_program);
        glBindVertexArray(array_obj);
        send_modulation(shader_program);
        for (int i=0; i<num_boxes; i++)
        {
            update(shader_program, boxes[i], i, delta_time); // Update model, view, projection matrices
            glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0);
        }

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Free resources
    glDeleteVertexArrays(1, &array_obj);
    glDeleteBuffers(1, &vbuf);
    glDeleteBuffers(1, &ibuf);
    glDeleteProgram(shader_program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
