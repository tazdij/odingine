#ifndef __SHADERS_H__
#define __SHADERS_H__



#include <SDL.h>
#include <GL/glew.h>
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
    unsigned char* name;
} ODIN_Shader;

ODIN_ShaderSource* Shaders_newShaderSource(int shader_type, unsigned char* source);
int Shaders_freeShaderSource(ODIN_ShaderSource* obj);

ODIN_Shader* Shaders_newShader(unsigned char* name, ODIN_ShaderSource* vert_shader, ODIN_ShaderSource* frag_shader);
int Shaders_freeShader(ODIN_Shader* shader);

#endif