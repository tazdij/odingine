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

ODIN_Shader* Shaders_newShader(unsigned char* name, ODIN_ShaderSource* vert_shader, ODIN_ShaderSource* frag_shader) {
    printf("Shaders_newShader()\n");
    ODIN_Shader* shader = (ODIN_Shader*)malloc(sizeof(ODIN_Shader));

    printf("Shaders_newShader()\n");

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);


    printf("Shader IDs created\n");

    GLint result = GL_FALSE;
    int infoLogLength = 0;

    // Compile the vert shader
    glShaderSource(vertShaderID, 1, (const char**)&(vert_shader->shader_source), NULL);
    glCompileShader(vertShaderID);

    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		unsigned char* vertexShaderErrorMessage = (unsigned char*)malloc(sizeof(unsigned char) * (infoLogLength+1));
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
		unsigned char* fragmentShaderErrorMessage = (unsigned char*)malloc(sizeof(unsigned char) * (infoLogLength+1));
		glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, fragmentShaderErrorMessage);
		printf("VertexShader Error: %s\n", fragmentShaderErrorMessage);
        free(fragmentShaderErrorMessage);
	}

    // TODO: Link the GL Shader Program

    shader->name = (unsigned char*)malloc(sizeof(unsigned char) * (strlen(name) + 1));
    memcpy(shader->name, name, sizeof(unsigned char) * (strlen(name) + 1));

    return 0;
}

int Shaders_freeShader(ODIN_Shader* shader) {

}