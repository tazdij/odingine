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

typedef struct ODIN_Texture {
    GLuint texture_id;
    GLenum texture_type;
    int width;
    int height;
    
} ODIN_Texture;

#endif /* textures_h */
