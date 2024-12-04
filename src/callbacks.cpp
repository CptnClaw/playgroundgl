#include <iostream>
#include "callbacks.h"
#include "shaders.h"

#define PI 3.14159f

void error_callback(int err, const char *msg)
{
    std::cout << "Error " << err << ": " << msg << std::endl;
}

void modify_by_action(int action, int value, int &to_modify)
{
    if (action == GLFW_PRESS)   to_modify = value;
    if (action == GLFW_RELEASE) to_modify = 0;
    // else do nothing
}

bool is_wireframe = false;
int move_x = 0, move_y = 0;
float rot_speed = .1f;
float delta = .05f;
float active_range = 0.5f;

bool mouse_entered = false;
double last_mouse_x, last_mouse_y;
float pitch = 0.f, yaw = PI / 2.f;
float mouse_sensitivity = .01f;
float zoom = 1;
float scroll_sensitivity = 0.1f;

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
    switch (key)
    {
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_CAPS_LOCK:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS)
        {
            if (is_wireframe)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            is_wireframe = !is_wireframe;
        }
        break;
    case GLFW_KEY_W:
        modify_by_action(action, 1, move_y);
        break;
    case GLFW_KEY_A:
        modify_by_action(action, -1, move_x);
        break;
    case GLFW_KEY_S:
        modify_by_action(action, -1, move_y);
        break;
    case GLFW_KEY_D:
        modify_by_action(action, 1, move_x);
        break;
    case GLFW_KEY_EQUAL:
        active_range += delta;
        if (active_range > 1.0f) active_range = 1.0f;
        std::cout << active_range << std::endl;
        break;
    case GLFW_KEY_MINUS:
        active_range -= delta;
        if (active_range < 0.0f) active_range = 0.0f;
        std::cout << active_range << std::endl;
        break;
    default:
        break;
    }
}

void fb_sz_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback([[maybe_unused]] GLFWwindow *window, double x, double y)
{
    if (mouse_entered)
    {
        pitch += (float)(last_mouse_y - y) * mouse_sensitivity;
        yaw += (float)(x - last_mouse_x) * mouse_sensitivity;
    }
    mouse_entered = true;
    last_mouse_x = x;
    last_mouse_y = y;
}

void scroll_callback([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] double xoffset, double yoffset)
{
    zoom -= yoffset * scroll_sensitivity;
}