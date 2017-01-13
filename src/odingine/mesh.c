#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Mesh_bindVbo(ODIN_Mesh* mesh, ODIN_UByte location) {
    
}

void Mesh_unbindVbo(GLenum buffer_type) {
    glBindBuffer(buffer_type, 0);
}

int Mesh_isIndexed(ODIN_Mesh* mesh) {
    if (mesh->ibo_id != 0) {
        return 1;
    }
    
    return 0;
}

ODIN_Mesh* Mesh_newMesh() {
	// Create the mesh struct
    ODIN_Mesh* mesh = (ODIN_Mesh*)malloc(sizeof(ODIN_Mesh));

    // Create the VAO
	glGenVertexArrays(1, &(mesh->vao_id));
    
    // Initialize the buffers array for all 16
    mesh->buffers = (ODIN_MeshBuffer*)malloc(sizeof(ODIN_MeshBuffer) * 16);
	
    mesh->ibo_count = 0;
    mesh->ibo_id = 0;

	return mesh;
}

void Mesh_freeMesh(ODIN_Mesh* mesh) {
    // TODO: Free all of the buffers in Video RAM
    // TODO: Free IBO in Video RAM
    // TODO: Free Buffer array
    // TODO: Free IBO
    // TODO: Free VAO in Video Ram
    
    // Free Mesh struct
	free(mesh);
}

// load the index buffer for this mesh
void Mesh_loadIBOData(ODIN_Mesh* mesh, GLuint* data, GLsizei count) {
    // Bind the VAO
    Mesh_bindVao(mesh);
    
    mesh->ibo_count = count;
    glGenBuffers(1, &(mesh->ibo_id));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    Mesh_unbindVao();
}

void Mesh_loadBufferData(ODIN_Mesh* mesh, GLuint location, GLenum buffer_type,
                       GLenum element_type, GLsizei element_size, GLuint component_count,
                       void* data, GLsizei data_count) {
    
    Mesh_bindVao(mesh);
    
    // Get the pointer to the element in the buffer array
    ODIN_MeshBuffer* buffer = mesh->buffers + location;
    
    buffer->element_type = element_type;
    buffer->element_size = element_size;
    buffer->buffer_type = buffer_type;
    buffer->component_count = component_count;
    buffer->buffer_location = location;
    
    glGenBuffers(1, &buffer->vbo_id);
    glBindBuffer(buffer_type, buffer->vbo_id);
    glBufferData(buffer_type, data_count * element_size, data, GL_STATIC_DRAW);
    
    
    //glEnableVertexAttribArray(index);
    //glVertexAttribPointer(index, buffer->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
    
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, buffer->component_count, element_type, GL_FALSE, 0, 0);
    
    Mesh_unbindVbo(buffer_type);
    Mesh_unbindVao();
}

void Mesh_bindVao(ODIN_Mesh* mesh) {
	glBindVertexArray(mesh->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo_id);
}

void Mesh_unbindVao() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}