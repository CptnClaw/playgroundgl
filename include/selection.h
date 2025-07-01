#ifndef SELECTION_H_
#define SELECTION_H_

#include <glm/glm.hpp>

// A mechanism for selecting objects by rendering to a texture object IDs in a second render pass
class Selection
{
public:
    Selection(uint width, uint height, bool &success);
    virtual ~Selection();

    // Do not allow implicit copy due to OpenGL resource management
    Selection(const Selection&) = delete;
    Selection& operator=(const Selection&) = delete;
    
    // Attaches selection framebuffer, so that following draw calls render off-screen to selection buffer
    void start() const;
    
    // Re-attaches to default framebuffer so that following draw calls render to screen
    void end() const;
    
    // Gets the index of the object at the (x,y) screen coordinate
    // Must be called only between start() and end()
    uint object_at(int x, int y) const;

private:
    // OpenGL stuff
    uint fbo; // framebuffer
    uint texture_id; // color buffer
    uint renderbuffer_id; // depth+stencil buffer
};

#endif // SELECTION_H_
