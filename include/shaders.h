#ifndef SHADERS_H_
#define SHADERS_H_

#include <string>
#include <glm/glm.hpp>
#include "texture.h"

class Shaders
{
    public:
        // Reads, compiles and links shader program
        // Make sure to check last argument for any errors
        Shaders(const std::string &vertex_shader_path, const std::string &fragment_shader_path, bool &success);

        // Frees resources
        ~Shaders();

        // Activate program
        void use() const;

        // Send uniform data to shaders
        void uniform_vec3(const std::string &uniform_name, glm::vec3 v) const;
        void uniform_vec4(const std::string &uniform_name, glm::vec4 v) const;
        void uniform_mat4(const std::string &uniform_name, glm::mat4 matrix) const;
        void uniform_mat3(const std::string &uniform_name, glm::mat3 matrix) const;
        void uniform_float(const std::string &uniform_name, float f) const;
        void uniform_modulation(const std::string &uniform_name) const;
        void uniform_int(const std::string &uniform_name, int i) const;

    private:
        int id; // OpenGL program index
};

#endif // SHADERS_H_
