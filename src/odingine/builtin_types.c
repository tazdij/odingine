#include "builtin_types.h"

#include "mpc.h"

#include "textures.h"
#include "mesh.h"
#include "shaders.h"

#include "viking_types.h"

#include <stdio.h>
#include <stdlib.h>


int BitmapResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// TODO: Load the resource from the rawdata
	printf("BitmapResource_Loader called: resource file = %s\n", resource->fname);

	// Create the Texture

	// Read the Bitmap
	unsigned char header[54];
	unsigned int dataPos = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int imageSize = 0;
	unsigned char* data = 0;

	// Get the header from the passed in data
	memcpy(header, rawdata, sizeof(unsigned char) * 54);

	if (header[0] != 'B' || header[1] != 'M') {
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	// Allocate the Memory needed for the image
	data = (unsigned char*)malloc(sizeof(unsigned char) * size);
	memcpy((void*)data, rawdata, sizeof(unsigned char) * size);

	resource->buffer = (void*)Textures_createTexture(width, height, GL_BGR, size, rawdata);
	resource->buffer_size = sizeof(ODIN_Texture);

	//TODO: Get the complete size of the resource in memory

	free(data);

	return 1;
}

int TextureResource_Unloader(ODIN_Resource* res) {

	return 0;
}


int TextResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// TODO: Load the resource from the rawdata
	// Since the text file does not contain the null terminator, we need to add it after
	// copying the rawdata into our resource buffer
	printf("TextResource_Loader called: resource file = %s\n", resource->fname);

	resource->buffer_size = sizeof(unsigned char) * size;
	resource->total_bytes = sizeof(unsigned char) * size;

	resource->buffer = (void*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)resource->buffer, rawdata, sizeof(unsigned char) * (size + 1));
	char* text = (char*)resource->buffer;
	text[size] = 0; // add the null terminator

	return 1;
}

int TextResource_Unloader(ODIN_Resource* resource) {
	resource->buffer_size = 0;
	resource->total_bytes = 0;
	free(resource->buffer);
	resource->buffer = 0;

	return 1;
}

int VertexShaderResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// A Shader is a source file that is loaded at runtime (which loads the actual shader parts)
	// and sent to the GPU to be compiled. Odingine has a single
	// struct that is used to manage and cache it's 
	// different information needed during runtime, to avoid round-trips
	// with the GPU

	unsigned char* tmpSource = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)tmpSource, rawdata, sizeof(unsigned char) * (size + 1));
	tmpSource[size] = 0;

	resource->buffer = (void*)Shaders_newShaderSource(ODIN_SHADERTYPE_VERTEX, tmpSource);

	resource->total_bytes = sizeof(ODIN_ShaderSource) + (sizeof(unsigned char) * (size + 1));
	resource->buffer_size = sizeof(ODIN_ShaderSource);

	free(tmpSource);

	return 1;
}

int FragmentShaderResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {

	unsigned char* tmpSource = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)tmpSource, rawdata, sizeof(unsigned char) * (size + 1));
	tmpSource[size] = 0;

	resource->buffer = (void*)Shaders_newShaderSource(ODIN_SHADERTYPE_FRAGMENT, tmpSource);
	resource->total_bytes = sizeof(ODIN_ShaderSource) + (sizeof(unsigned char) * (size + 1));
	resource->buffer_size = sizeof(ODIN_ShaderSource);

	free(tmpSource);

	return 1;
}

int ShaderSourceResource_Unloader(ODIN_Resource* resource) {

	resource->buffer_size = 0;
	resource->total_bytes = 0;
	Shaders_freeShaderSource((ODIN_ShaderSource*)resource->buffer);
	resource->buffer = 0;

	return 1;
}


void BuiltinTypes_init() {
	//TODO: Register all of the loaders and unloaders here

	
	
	

	// Texture Loaders
	ODIN_UInt64 RESTYPE_TEXTURE = Resources_registerType();
	Resources_registerLoader(RESTYPE_TEXTURE, &BitmapResource_Loader, "*.bmp");
	//Resources_registerLoader(RESTYPE_TEXTURE, &TGAResource_Loader, "*.tga");
	//Resources_registerLoader(RESTYPE_TEXTURE, &DDSResource_Loader, "*.dds");
	Resources_registerUnloader(RESTYPE_TEXTURE, &TextureResource_Unloader);

	// Text Loaders
	ODIN_UInt64 RESTYPE_TEXT = Resources_registerType();
	Resources_registerLoader(RESTYPE_TEXT, &TextResource_Loader, "*.txt");
	Resources_registerUnloader(RESTYPE_TEXT, &TextResource_Unloader);

	// Shader Loaders
	ODIN_UInt64 RESTYPE_SHADERSRC = Resources_registerType();
	Resources_registerLoader(RESTYPE_SHADERSRC, &VertexShaderResource_Loader, "*.vs");
	Resources_registerLoader(RESTYPE_SHADERSRC, &FragmentShaderResource_Loader, "*.fs");
	Resources_registerUnloader(RESTYPE_SHADERSRC, &ShaderSourceResource_Unloader);
}