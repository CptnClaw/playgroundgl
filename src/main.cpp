#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"
#include "shaders.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main()
{
    /* Initialize the library */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSwapInterval(1);
    
    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, fb_sz_callback);

    // Compile shaders
    uint shader_program[2];
    shader_program[0] = build_program("src/vertex.glsl", "src/fragment.glsl");
    if (0 == shader_program[0])
    {
        glfwTerminate();
        return -1;
    }
    shader_program[1] = build_program("src/vertex.glsl", "src/fragment2.glsl");
    if (0 == shader_program[1])
    {
        glfwTerminate();
        return -1;
    }

    // Define triangles
    float vertices[] = {
        -0.5f, -0.5f, 
         0.5f, -0.5f, 
         0.f,   0.5f,
         -0.5f, 0.5f,
         0.5f,  0.5f,
         0.f,  -0.5f
    };
    uint indices1[] = {
        0, 1, 2
    };
    uint indices2[] = {
        3, 4, 5
    };

    // Send vertices to GPU
    uint vbuf;
    uint ibuf[2];
    uint array_objs[2];
    glGenBuffers(1, &vbuf);
    glGenBuffers(2, ibuf);
    glGenVertexArrays(2, array_objs);

    glBindVertexArray(array_objs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(array_objs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Run shaders
        for (int i=0; i<=1; i++)
        {
            glUseProgram(shader_program[i]);
            glBindVertexArray(array_objs[i]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        }

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, array_objs);
    glDeleteBuffers(1, &vbuf);
    glDeleteBuffers(2, ibuf);
    glDeleteProgram(shader_program[0]);
    glDeleteProgram(shader_program[1]);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}