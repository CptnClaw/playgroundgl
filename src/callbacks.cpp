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
bool is_flashlight = false;
bool is_sun = true;
int move_x = 0, move_y = 0;
float rot_speed = 0.f;
float delta = .1f;
float light_strength = 0.5f;

bool mouse_entered_focus = true;
double last_mouse_x, last_mouse_y;
float camera_pitch = 0.f, camera_yaw = PI / 2.f;
float flashlight_pitch = 0.f, flashlight_yaw = -PI / 2.f;
float mouse_sensitivity = .005f;
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
    case GLFW_KEY_1:
        // Toggle wireframe
        if (action == GLFW_PRESS)
        {
            if (is_wireframe)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            is_wireframe = !is_wireframe;
        }
        break;
    case GLFW_KEY_2:
        // Toggle flashlight
        if (action == GLFW_PRESS)
        {
            is_flashlight = !is_flashlight;
            flashlight_pitch = 0.f;
            flashlight_yaw = -PI / 2.f;
        }
        break;
    case GLFW_KEY_3:
        // Toggle sun
        if (action == GLFW_PRESS)
        {
            is_sun = !is_sun;
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
        rot_speed += delta;
        break;
    case GLFW_KEY_MINUS:
        rot_speed -= delta;
        break;
    case GLFW_KEY_K:
        light_strength += delta;
        if (light_strength > 1.0f) light_strength = 1.0f;
        break;
    case GLFW_KEY_J:
        light_strength -= delta;
        if (light_strength < 0.0f) light_strength = 0.0f;
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
    // Choose if mouse movements are tied to camera or flashlight
    // (state machine)
    float *modifier_pitch = &camera_pitch;
    float *modifier_yaw = &camera_yaw;
    if (is_flashlight)
    {
        modifier_pitch = &flashlight_pitch;
        modifier_yaw = &flashlight_yaw;
    }

    // Update selected pitch and yaw values according to mouse movements
    // Except for when mouse just entered focus, to avoid sudden jumps
    if (!mouse_entered_focus)
    {
        *modifier_pitch += (float)(last_mouse_y - y) * mouse_sensitivity;
        *modifier_yaw += (float)(x - last_mouse_x) * mouse_sensitivity;
    }
    
    // Remember current mouse coordinates
    mouse_entered_focus = false;
    last_mouse_x = x;
    last_mouse_y = y;
}

void scroll_callback([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] double xoffset, double yoffset)
{
    zoom -= yoffset * scroll_sensitivity;
}

void focus_callback([[maybe_unused]] GLFWwindow *window, int focused)
{
    if (focused)
    {
        // Lock mouse only after one window defocusing event.
        // Workaround for a hyprland/wayland bug?
        if (!mouse_entered_focus) 
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        mouse_entered_focus = true;
    }
    else
    {
        mouse_entered_focus = false;
    }
}
