#include "window.h"
#include <stdlib.h>
#include <stdio.h>

ODIN_Window* Window_new(const char* title, const int width, const int height) {
    ODIN_Window* odin_window = (ODIN_Window*)malloc(sizeof(ODIN_Window));

    //TODO: Configure for mobile

    // Configure the OpenGL Version DESKTOP
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Temp set pointer to 0 (NULL) - to point at nothing
    odin_window->window = 0;
    odin_window->window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_OPENGL);

    // Check if window is created
	if (odin_window->window == 0) return 0;

	// Create the actual OpenGL Context
	odin_window->context = SDL_GL_CreateContext(odin_window->window);

	// Make GL Context Current
	SDL_GL_MakeCurrent(odin_window->window, odin_window->context);

    // Set VSync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! Error: %s\n", SDL_GetError());
    }

    printf("OpenGL %s\n", glGetString(GL_VERSION));

    if (glewInit() != GLEW_OK) {
        printf("Error initializing GLEW!\n");
        return 0;
    }
    

    return odin_window;
}

void Window_delete(ODIN_Window* odin_window) {
    SDL_GL_DeleteContext(odin_window->context);
    SDL_DestroyWindow(odin_window->window);
    free(odin_window);
}

void Window_clearColor(const float r, const float g, const float b, const float a) {
    // OpenGL Rendering loop
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window_swapBuffer(ODIN_Window* window) {
    SDL_GL_SwapWindow(window->window);
}