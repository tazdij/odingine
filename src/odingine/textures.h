//
//  textures.h
//  Engine
//
//  Created by Donald Duvall on 9/12/16.
//
//

#ifndef textures_h
#define textures_h

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "viking_types.h"

typedef struct ODIN_Texture {
    GLuint texture_id;
    GLenum texture_type;
    ODIN_UInt32 width;
    ODIN_UInt32 height;
} ODIN_Texture;



ODIN_Texture* Textures_createTexture(ODIN_UInt32 width, ODIN_UInt32 height, GLenum format, ODIN_UInt64 size, unsigned char * rawdata);
int Textures_freeTexture(ODIN_Texture* tex);

#endif /* textures_h */
