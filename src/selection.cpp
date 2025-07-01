#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "selection.h"

Selection::Selection(uint width, uint height, bool &success)
{
    success = true;

    // Create and bind framebuffer 
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create and attach texture (color buffer)
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    
    // Create and attach renderbuffer (depth+stencil buffer)
    glGenRenderbuffers(1, &renderbuffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_id);
    
    // Check for errors
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Error: Incomplete framebuffer" << std::endl;
        success = false;
    }
    
    // Rebind default framebuffer for on-screen rendering
    end();
}

Selection::~Selection()
{
    std::cout << "NOTE: deleteing framebuffer " << fbo << " and its attached buffers" << std::endl;
    glDeleteRenderbuffers(1, &renderbuffer_id);
    glDeleteTextures(1, &texture_id);
    glDeleteFramebuffers(1, &fbo);
}

void Selection::start() const
{
    // Bind framebuffer and clear it 
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Selection::end() const
{
    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint Selection::object_at(int x, int y) const
{
    uint data;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &data);
    return data;
}
