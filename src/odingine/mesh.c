#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Mesh_bindVbo(ODIN_Mesh* mesh, ODIN_UByte location) {
    
}

void Mesh_unbindVbo(GLenum buffer_type) {
    glBindBuffer(buffer_type, 0);
}

ODIN_Mesh* Mesh_newMesh() {
	ODIN_Mesh* mesh = (ODIN_Mesh*)malloc(sizeof(ODIN_Mesh));

	glGenVertexArrays(1, &(mesh->vao_id));
    
    // initialize the buffers array
    mesh->buffers = (ODIN_MeshBuffer*)malloc(sizeof(ODIN_MeshBuffer) * 16);
	

	
	//mesh->vao_id = 0;

	return mesh;
}

void Mesh_freeMesh(ODIN_Mesh* mesh) {
	free(mesh);
}

void Mesh_loadBuffer(ODIN_Mesh* mesh, GLuint location, GLenum buffer_type,
                       GLenum element_type, GLubyte element_size, GLuint element_count,
                       void* data) {
    
    Mesh_bindVao(mesh);
    
    ODIN_MeshBuffer* buffer = mesh->buffers;
    
    glGenBuffers(1, &buffer->vbo_id);
    glBindBuffer(buffer_type, buffer->vbo_id);
    glBufferData(buffer_type, buffer->element_count * element_size, data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, buffer->element_count, element_type, GL_FALSE, 0, 0);
    
    Mesh_unbindVbo(buffer_type);
    Mesh_unbindVao();
}

void Mesh_bindVao(ODIN_Mesh* mesh) {
	glBindVertexArray(mesh->vao_id);
}

void Mesh_unbindVao() {
	glBindVertexArray(0);
}