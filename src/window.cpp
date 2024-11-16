#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "callbacks.h"

Window::Window(uint width, uint height, bool &success)
{
    // Initialize the library 
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        is_initialized = success = false;
        return;
    }
    is_initialized = true;

    // Create a windowed mode window and its OpenGL context 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    handle = glfwCreateWindow(width, height, "playgroundgl", nullptr, nullptr);
    if (!handle)
    {
        std::cout << "Failed to open a window" << std::endl;
        success = false;
        return;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(handle);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        success = false;
        return;
    }
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set callbacks
    glfwSetKeyCallback(handle, key_callback);
    glfwSetFramebufferSizeCallback(handle, fb_sz_callback);
    glfwSetCursorPosCallback(handle, mouse_callback);
    glfwSetScrollCallback(handle, scroll_callback);
    
    success = true;
}

Window::~Window()
{
    if (is_initialized)
    {
        if (handle != nullptr)
        {
            glfwDestroyWindow(handle);
        }
        glfwTerminate();
    }
}

bool Window::next_frame_ready() const
{
    // Swap front and back buffers
    glfwSwapBuffers(handle);

    // Poll for and process events
    glfwPollEvents();

    if (glfwWindowShouldClose(handle))
    {
        return false;
    }

    // Clear screen
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}
