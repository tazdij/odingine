#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#define ODIN_SHADERTYPE_VERTEX GL_VERTEX_SHADER
#define ODIN_SHADERTYPE_GEOMETRY GL_GEOMETRY_SHADER
#define ODIN_SHADERTYPE_FRAGMENT GL_FRAGMENT_SHADER

typedef struct ODIN_ShaderSource {
    int shader_type;
    unsigned char* shader_source;
} ODIN_ShaderSource;

typedef struct ODIN_Shader {
    GLuint program_id;
} ODIN_Shader;

ODIN_ShaderSource* Shaders_newShaderSource(int shader_type, unsigned char* source);
int Shaders_freeShaderSource(ODIN_ShaderSource* obj);


#endif