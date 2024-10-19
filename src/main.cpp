#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main()
{
    int success;
    char compilation_errs[512];
    FILE *shader_file;
    char shader_txt[1024];

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

    // Compile vertex shader
    uint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    shader_file = fopen("src/vertex.glsl", "r");
    if (!shader_file)
    {
        std::cout << "Cannot find vertex.glsl shader file" << std::endl;
        glfwTerminate();
        return -1;
    }
    int filesize = fread(shader_txt, sizeof(char), 1024, shader_file);
    shader_txt[filesize] = '\0';
    fclose(shader_file);
    const char *vertex_shader_src = shader_txt;
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, compilation_errs);
        std::cout << "Error compiling vertex shader: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        glfwTerminate();
        return -1;
    }

    // Compile fragment shader
    uint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    shader_file = fopen("src/fragment.glsl", "r");
    if (!shader_file)
    {
        std::cout << "Cannot find fragment.glsl shader file" << std::endl;
        glfwTerminate();
        return -1;
    }
    filesize = fread(shader_txt, sizeof(char), 1024, shader_file);
    shader_txt[filesize] = '\0';
    fclose(shader_file);
    const char *fragment_shader_src = shader_txt;
    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, compilation_errs);
        std::cout << "Error compiling fragment shader: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        glfwTerminate();
        return -1;
    }
    
    uint fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    shader_file = fopen("src/fragment2.glsl", "r");
    if (!shader_file)
    {
        std::cout << "Cannot find fragment2.glsl shader file" << std::endl;
        glfwTerminate();
        return -1;
    }
    filesize = fread(shader_txt, sizeof(char), 1024, shader_file);
    shader_txt[filesize] = '\0';
    fclose(shader_file);
    const char *fragment_shader2_src = shader_txt;
    glShaderSource(fragment_shader2, 1, &fragment_shader2_src, nullptr);
    glCompileShader(fragment_shader2);
    glGetShaderiv(fragment_shader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader2, 512, nullptr, compilation_errs);
        std::cout << "Error compiling fragment shader: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        glfwTerminate();
        return -1;
    }

    // Link shader program
    uint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, nullptr, compilation_errs);
        std::cout << "Error linking shader program: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        glfwTerminate();
        return -1;
    }
    uint shader_program2 = glCreateProgram();
    glAttachShader(shader_program2, vertex_shader);
    glAttachShader(shader_program2, fragment_shader2);
    glLinkProgram(shader_program2);
    glGetProgramiv(shader_program2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program2, 512, nullptr, compilation_errs);
        std::cout << "Error linking shader program: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        glfwTerminate();
        return -1;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(fragment_shader2);

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
    uint vbuf1, ibuf1, array1;
    uint ibuf2, array2;
    glGenBuffers(1, &vbuf1);
    glGenBuffers(1, &ibuf1);
    glGenVertexArrays(1, &array1);
    glGenBuffers(1, &ibuf2);
    glGenVertexArrays(1, &array2);

    glBindVertexArray(array1);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(array2);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf1);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(array1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glUseProgram(shader_program2);
        glBindVertexArray(array2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &array1);
    glDeleteVertexArrays(1, &array2);
    glDeleteBuffers(1, &vbuf1);
    glDeleteBuffers(1, &ibuf1);
    glDeleteBuffers(1, &ibuf2);
    glDeleteProgram(shader_program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}