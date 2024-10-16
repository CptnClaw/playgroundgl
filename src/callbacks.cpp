#include <iostream>
#include "callbacks.h"

void error_callback(int err, const char *msg)
{
    std::cout << "Error " << err << ": " << msg << std::endl;
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
    if (GLFW_KEY_Q == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void fb_sz_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}