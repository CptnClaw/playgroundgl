#include <iostream>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "cubemap.h"


CubeMap::CubeMap(std::string texture_directory)
{
    // Prepare OpenGL texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Prepare individual texture paths
    std::vector<std::string> paths = {"/right.jpg", "/left.jpg", "/top.jpg", "/bottom.jpg", "/front.jpg", "/back.jpg"};

    // Read image files
    int texwidth, texheight, texchannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *texdata; 
    for (size_t i = 0; i < paths.size(); i++)
    {
        std::string cur_path = texture_directory + paths[i];
        texdata = stbi_load(cur_path.c_str(), &texwidth, &texheight, &texchannels, 0);

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
            std::cout << " in texture " << cur_path << std::endl;
            break;
        }

        // Copy image into GPU
        if (texdata)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texwidth, texheight, 0, format, GL_UNSIGNED_BYTE, texdata);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
            std::cout << cur_path << std::endl;
        }
        
        stbi_image_free(texdata);
    }
}

CubeMap::~CubeMap()
{
    std::cout << "NOTE: deleting cubemap " << id << std::endl;
    glDeleteTextures(1, &id);
}


void CubeMap::activate(uint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}