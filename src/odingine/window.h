#ifndef WINDOW_H
#define WINDOW_H


#include <SDL.h>

    #include <GL/glew.h>

#include <SDL_opengl.h>
//#include <SDL_rwops.h>
//#include "SDL_local.h"

/*
    This is the window utility, which will setup our SDL context
*/

typedef struct ODIN_Window {
    SDL_Window* window;
    SDL_GLContext context;
} ODIN_Window;

ODIN_Window* Window_new(const char* title, const int width, const int height);
void Window_delete(ODIN_Window* window);

void Window_clearColor(const float r, const float g, const float b, const float a);
void Window_swapBuffer(ODIN_Window* window);


#endif