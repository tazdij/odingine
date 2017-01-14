#include "shaders.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



ODIN_ShaderSource* Shaders_newShaderSource(int shader_type, unsigned char* source) {
    ODIN_ShaderSource* src = (ODIN_ShaderSource*)malloc(sizeof(ODIN_ShaderSource));

    src->shader_source = (unsigned char*)malloc(sizeof(unsigned char) * (strlen(source) + 1));
    memcpy(src->shader_source, source, sizeof(unsigned char) * (strlen(source) + 1));

    src->shader_type = shader_type;

    return src;
}

int Shaders_freeShaderSource(ODIN_ShaderSource* obj) {
	free(obj->shader_source);
	free(obj);

	return 1;
}

//typedef struct ODIN_Shader {
//    GLuint program_id;
//    unsigned char* name;
//} ODIN_Shader;

ODIN_Shader* Shaders_newShader(const unsigned char* name, ODIN_ShaderSource* vert_shader, ODIN_ShaderSource* frag_shader) {
    printf("Shaders_newShader()\n");
    ODIN_Shader* shader = (ODIN_Shader*)malloc(sizeof(ODIN_Shader));

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int infoLogLength = 0;

    // Compile the vert shader
    glShaderSource(vertShaderID, 1, (const char**)&(vert_shader->shader_source), NULL);
    glCompileShader(vertShaderID);

    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		GLchar* vertexShaderErrorMessage = (GLchar*)calloc(infoLogLength + 1, sizeof(GLchar));
		glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, vertexShaderErrorMessage);
		printf("VertexShader Error: %s\n", vertexShaderErrorMessage);
        free(vertexShaderErrorMessage);
	}

    // Compile the vert shader
    glShaderSource(fragShaderID, 1, (const char**)&(frag_shader->shader_source), NULL);
    glCompileShader(fragShaderID);

    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
        GLchar* fragmentShaderErrorMessage = (GLchar*)calloc(infoLogLength + 1, sizeof(GLchar));
		glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, fragmentShaderErrorMessage);
		printf("VertexShader Error: %s\n", fragmentShaderErrorMessage);
        free(fragmentShaderErrorMessage);
	}

    // TODO: Link the GL Shader Program
    printf("Linking Shader Program.\n");
    shader->program_id = glCreateProgram();
    
    glAttachShader(shader->program_id, vertShaderID);
    glAttachShader(shader->program_id, fragShaderID);
    glLinkProgram(shader->program_id);
    
    // Chech program for errors
    glGetProgramiv(shader->program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(shader->program_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ) {
        GLchar* programErrorMessage = (GLchar*)calloc(infoLogLength + 1, sizeof(GLchar));
        glGetProgramInfoLog(shader->program_id, infoLogLength, NULL, programErrorMessage);
        printf("Shader Program Error: %s\n", programErrorMessage);
        free(programErrorMessage);
    }
    
    glDetachShader(shader->program_id, vertShaderID);
    glDetachShader(shader->program_id, fragShaderID);
    
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    shader->name = (unsigned char*)calloc(strlen((const char*)name) + 1, sizeof(unsigned char));
    memcpy(shader->name, name, sizeof(unsigned char) * strlen(name)); // All bytes are already null, no need to copy it too

    return shader;
}

int Shaders_freeShader(ODIN_Shader* shader) {
    // Free the  name
    free(shader->name);
    
    glDeleteProgram(shader->program_id);
    free(shader);
    
    return 1;
}

void Shaders_setShader(ODIN_Shader* shader) {
	glUseProgram(shader->program_id);
}

void Shaders_unsetShader() {
	glUseProgram(0);
}


GLint Shaders_getUniformLocation(ODIN_Shader* shader, const GLchar* name) {
	// TODO: add fetch from cached map

	return glGetUniformLocation(shader->program_id, name);
}

void Shaders_setUniform1f(ODIN_Shader* shader, const GLchar* name, float val) {
	glUniform1f(Shaders_getUniformLocation(shader, name), val);
}

void Shaders_setUniform1i(ODIN_Shader* shader, const GLchar* name, int val) {
	glUniform1i(Shaders_getUniformLocation(shader, name), val);
}

void Shaders_setUniform2f(ODIN_Shader* shader, const GLchar* name, vec3_t* vec) {
	glUniform2fv(Shaders_getUniformLocation(shader, name), 2, vec);
}

void Shaders_setUniform3f(ODIN_Shader* shader, const GLchar* name, vec3_t* vec) {
	glUniform3fv(Shaders_getUniformLocation(shader, name), 3, vec);
}

void Shaders_setUniform4f(ODIN_Shader* shader, const GLchar* name, vec4_t* vec) {
	glUniform4fv(Shaders_getUniformLocation(shader, name), 4, vec);
}

void Shaders_setUniformMat4(ODIN_Shader* shader, const GLchar* name, mat4_t* mat) {
	glUniformMatrix4fv(Shaders_getUniformLocation(shader, name), 1, GL_FALSE, mat);
}