//
//  textures.c
//  Engine
//
//  Created by Donald Duvall on 9/12/16.
//
//
#include "textures.h"
#include <stdio.h>


/*
typedef struct ODIN_Texture {
	GLuint texture_id;
	GLenum texture_type;
	int width;
	int height;

} ODIN_Texture;
*/


ODIN_Texture* Textures_createTexture(ODIN_UInt32 width, ODIN_UInt32 height, GLenum format, ODIN_UInt64 size, unsigned char * rawdata) {
	
	ODIN_Texture* tex = (ODIN_Texture*)malloc(sizeof(ODIN_Texture));

	// Set the basic information


	// Tell OpenGL to create a texture
	glGenTextures(1, &(tex->texture_id));

	// Bind the new texture
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);

    // Pass the image data to
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, rawdata);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

int Textures_freeTexture(ODIN_Texture* tex) {
	glDeleteTextures(1, &(tex->texture_id));
	return 0;
}

void Textures_bindTexture(ODIN_Texture* tex, GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	
}

void Textures_unbindTexture(GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}