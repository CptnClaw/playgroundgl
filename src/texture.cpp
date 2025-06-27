#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "texture.h" 

Texture::Texture(const std::string &texture_path, TextureType type) : 
    filepath(texture_path), type(type)
{
    // Prepare OpenGL texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Read image file
    int texwidth, texheight, texchannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *texdata = stbi_load(texture_path.c_str(), &texwidth, &texheight, &texchannels, 0);

    // Infere RGB or RGBA from number of channels
    GLenum format = GL_RGB;
    switch (texchannels)
    {
    case 3:
        // Keep it GL_RGB
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        std::cout << "Error: unsupported number of channels " << texchannels;
        std::cout << " in texture " << texture_path << std::endl;
        break;
    }

    // Copy image into GPU
    if (texdata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, format, GL_UNSIGNED_BYTE, texdata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        std::cout << texture_path << std::endl;
    }
    
    stbi_image_free(texdata);
}

Texture::~Texture()
{
    std::cout << "NOTE: deleteing texture " << id << std::endl;
    glDeleteTextures(1, &id);
}

void Texture::activate(uint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}