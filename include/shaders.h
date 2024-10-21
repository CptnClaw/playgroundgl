#ifndef SHADERS_H_
#define SHADERS_H_

#include <string>

void send_movement(int shader_prog, float x, float y, float zoom);
void send_modulation(int shader_prog);
uint build_program(std::string vertex_shader_path, std::string fragment_shader_path);

#endif // SHADERS_H_
