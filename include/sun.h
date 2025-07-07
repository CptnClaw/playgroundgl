#ifndef SUN_H_
#define SUN_H_

#include <glm/glm.hpp>
#include "shaders.h"

class Sun
{
public:
    // Construct sun
    Sun(glm::vec3 direction);
    void use(const Shaders &program) const;

private:
    glm::vec3 direction;
};

#endif // SUN_H_