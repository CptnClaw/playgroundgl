#ifndef TEXTURE_H_ 
#define TEXTURE_H_

#include <string>

enum class TextureType 
{
    Diffuse,
    Specular
};

class Texture
{
public:
    // Load texture from image file
    Texture(const std::string &texture_path, TextureType type);
    
    // Do not allow implicit copy due to OpenGL resource management
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Free resources
    ~Texture();

    // Activate texture unit
    void activate(uint unit) const;

    // OpenGL index of texture
    uint id;

    // Misc members
    std::string filepath;
    TextureType type;
};


#endif // TEXTURE_H_
