#ifndef SHADERS_H_
#define SHADERS_H_

#include <string>
#include <glm/glm.hpp>

void send_matrix(int shader_prog, std::string uniform_name, glm::mat4 matrix);
void send_modulation(int shader_prog);
uint build_program(std::string vertex_shader_path, std::string fragment_shader_path);

#endif // SHADERS_H_
