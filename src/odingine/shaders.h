#ifndef __SHADERS_H__
#define __SHADERS_H__



#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include "viking_math.h"


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

ODIN_Shader* Shaders_newShader(const unsigned char* name, ODIN_ShaderSource* vert_shader, ODIN_ShaderSource* frag_shader);
int Shaders_freeShader(ODIN_Shader* shader);

void Shaders_setShader(ODIN_Shader* shader);
void Shaders_unsetShader();

void Shaders_setUniform1f(ODIN_Shader* shader, const GLchar* name, float val);
void Shaders_setUniform1i(ODIN_Shader* shader, const GLchar* name, int val);
void Shaders_setUniform2f(ODIN_Shader* shader, const GLchar* name, vec3_t* vec);
void Shaders_setUniform3f(ODIN_Shader* shader, const GLchar* name, vec3_t* vec);
void Shaders_setUniform4f(ODIN_Shader* shader, const GLchar* name, vec4_t* vec);
void Shaders_setUniformMat4(ODIN_Shader* shader, const GLchar* name, mat4_t* mat);

#endif