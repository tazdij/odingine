#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ODIN_Mesh* Mesh_newMesh() {
	ODIN_Mesh* mesh = (ODIN_Mesh*)malloc(sizeof(ODIN_Mesh));

	glGenVertexArrays(1, &(mesh->vao_id));
	

	
	//mesh->vao_id = 0;

	return mesh;
}

void Mesh_freeMesh(ODIN_Mesh* mesh) {
	free(mesh);
}

void Mesh_bindVao(ODIN_Mesh* mesh) {
	glBindVertexArray(mesh->vao_id);
}

void Mesh_unbindVao() {
	glBindVertexArray(0);
}