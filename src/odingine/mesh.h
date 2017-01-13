#ifndef __MESH_H__
#define __MESH_H__

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include "viking_types.h"

#define VBO_IDX_VERTICES 0
#define VBO_IDX_INDEX 1
#define VBO_IDX_UV_COORDINATES 2
#define VBO_IDX_COLOR 3

typedef struct ODIN_MeshBuffer {
	// ODIN_MeshBuffer,
	// This struct does not contain the data itself, however describes
	// the data that was inserted into the Graphics Card. It also contains
	// the "Pointer" (ID) to the data, for use by OpenGL, allowing us to
	// bind to the data when needed, or to free when finally finished

	GLuint vbo_id; // The id of the VBO returned from OpenGL (Sort of a pointer to it)
	GLubyte buffer_location; // The Location of this buffer in the VAO
	GLenum buffer_type; // The type of buffer GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
	GLubyte element_size; // The size of each element in bytes
	GLenum element_type; // The actual type of the data in the buffer
	GLuint component_count; // The number of elements per component (x, y, z) = 3
} ODIN_MeshBuffer;

typedef struct ODIN_Mesh {

	GLuint vao_id;
	
	// The index buffer will be managed by the Mesh directly
	GLsizei ibo_count; // Should this be the unique vertex count or the total number of indices?
	GLuint ibo_id;

    // buffers is always malloc'd for 16 elements.
	ODIN_MeshBuffer* buffers;
} ODIN_Mesh;


ODIN_Mesh* Mesh_newMesh();
void Mesh_freeMesh(ODIN_Mesh* mesh);

void Mesh_loadIBOData(ODIN_Mesh* mesh, GLuint* data, GLsizei count);

void Mesh_loadBufferData(ODIN_Mesh* mesh, GLuint location, GLenum buffer_type,
                         GLenum element_type, GLsizei element_size, GLuint component_count,
                         void* data, GLsizei data_count);

void Mesh_bindVao(ODIN_Mesh* mesh);
void Mesh_unbindVao();



#endif