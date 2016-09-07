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