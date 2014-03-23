#include <SDL.h>
#include <GL/glew.h>
#include "texture.h"

unsigned char* generate_debug_texture(int w,int h) {
        unsigned char* texture_data;
        int i;
        int addr=0;

        texture_data = malloc( w*h*4,
               sizeof(unsigned char));
        for(i=0; i<(w*h);++i) {
            int j=i/w;
            texture_data[addr++]=addr+j;
            texture_data[addr++]=addr+i;
            texture_data[addr++]=addr+(i*j);
            texture_data[addr++]=(i*i+j*j);
        }
        return texture_data;
}

#define _LOD 0 
#define _NOBORDER 0 

void texture_generate(gfx_texture* texture,int w,int h) {
    unsigned char* texture_data = generate_debug_texture(w,h);

    glGenTextures(1,&texture->texture_id);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,w,h ,_NOBORDER,
                GL_RGBA, GL_UNSIGNED_BYTE,texture_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    free(texture_data);
}

void texture_from_SDL_surface(gfx_texture* texture, SDL_Surface* surf) {
    SDL_LockSurface(surf);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,surf->w,surf->h ,_NOBORDER,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,(unsigned char*)surf->pixels);
    SDL_UnlockSurface(surf);

}

void texture_drop(gfx_texture* texture) {
    glDeleteTextures(1,&texture->texture_id);
}

void texture_bind(gfx_texture* texture, int texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture->texture_id );
}
