#include <iostream>
#include "callbacks.h"
#include "shaders.h"

void error_callback(int err, const char *msg)
{
    std::cout << "Error " << err << ": " << msg << std::endl;
}

bool is_wireframe = false;
float move_x = 0.f, move_y = 0.f, rot_speed = .1f;
float delta = .05f;

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
    if (GLFW_PRESS == action || GLFW_REPEAT == action)
    {
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_CAPS_LOCK:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_SPACE:
            if (is_wireframe)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);               
            is_wireframe = !is_wireframe;
            break;
        case GLFW_KEY_W:
            move_y += delta;
            break;
        case GLFW_KEY_A:
            move_x -= delta;
            break;
        case GLFW_KEY_S:
            move_y -= delta;
            break;
        case GLFW_KEY_D:
            move_x += delta;
            break;
        case GLFW_KEY_EQUAL:
            rot_speed += delta;
            break;
        case GLFW_KEY_MINUS:
            rot_speed -= delta;
            break;
        default:
            break;
        }
    }
}

void fb_sz_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}