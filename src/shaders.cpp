#include <iostream>
#include <math.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shaders.h"

uint shader_program[2];

void send_movement(int shader_prog, float x, float y, float zoom)
{
    int unif_loc = glGetUniformLocation(shader_prog, "movement");
    glUniform2f(unif_loc, x, y);
    unif_loc = glGetUniformLocation(shader_prog, "zoom");
    glUniform1f(unif_loc, zoom);
}

void send_modulation(int shader_prog)
{
    double time = glfwGetTime();
    double modulation = (sin(time) + 1) / 2.0;
    int unif_loc = glGetUniformLocation(shader_prog, "modulation");
    glUniform1f(unif_loc, modulation);
}

uint compile_shader(std::string shader_path, GLenum shader_type)
{
    int success;
    char compilation_errs[512];
    FILE *shader_file;
    char shader_txt[1024];

    // Read shader code
    shader_file = fopen(shader_path.c_str(), "r");
    if (!shader_file)
    {
        std::cout << "Cannot find shader file: " << shader_path << std::endl;
        return 0;
    }
    int filesize = fread(shader_txt, sizeof(char), 1024, shader_file);
    shader_txt[filesize] = '\0';
    fclose(shader_file);
    const char *shader_src = shader_txt;

    // Compile shader
    uint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, compilation_errs);
        std::cout << "Error compiling shader: " << std::endl;
        std::cout << compilation_errs << std::endl << std::endl;
        return 0;
    }
    return shader;
}

uint build_program(std::string vertex_shader_path, std::string fragment_shader_path)
{
    // Compile vertex shader
    uint vertex_shader = compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    if (0 == vertex_shader)
    {
        return 0;
    }

    // Compile fragment shader
    uint fragment_shader = compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    if (0 == fragment_shader)
    {
        return 0;
    }

    // Link shader program
    int success;
    char compilation_errs[512];
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
        return 0;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}