#include <GL/glew.h>

#include "framebuffer.h"

void framebuffer_create_framebuffer(gfx_framebuffer* framebuffer) {
    glGenFramebuffers(1, &framebuffer->framebuffer_id);
}

void framebuffer_drop(gfx_framebuffer* framebuffer) {
    glDeleteFramebuffers(1, &framebuffer->framebuffer_id);
}

void framebuffer_bind_texture(gfx_framebuffer* framebuffer, gfx_texture* texture) {
#define NUM_BUFS 1
    GLenum DrawBuffers[NUM_BUFS] = {GL_COLOR_ATTACHMENT0};
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->framebuffer_id );
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->texture_id,0);
    glDrawBuffers(NUM_BUFS, DrawBuffers); 
}

void framebuffer_render_start(gfx_framebuffer* framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->framebuffer_id );
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,2048,2048);
}

void framebuffer_render_end(gfx_framebuffer* framebuffer) {
#define NO_FRAMEBUFFER_BOUND 0
    glPopAttrib();
    glViewport(0,0,1920,1080);
    glBindFramebuffer(GL_FRAMEBUFFER, NO_FRAMEBUFFER_BOUND );
}
