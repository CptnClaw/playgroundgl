#ifndef TEXTURE_H_ 
#define TEXTURE_H_

#include <string>

class Texture
{
public:
    // Load texture from image file
    Texture(const std::string &texture_path, bool has_alpha_channel);

    // Free resources
    ~Texture();

    // Activate texture unit
    void activate(uint unit) const;

private:
    // OpenGL index of texture
    uint id;
};


#endif // TEXTURE_H_
