#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* mat)
{
	this->vertices = vertices;
	this->indices = indices;
	this->material = mat;

	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->vertexArrayObject);
	glDeleteBuffers(1, &this->vertexArrayBuffer);
	glDeleteBuffers(1, &this->vertexIndexBuffer);
}

void Mesh::SetupMesh()
{
	// Create vertexArray
	glGenVertexArrays(1, &this->vertexArrayObject);
	glBindVertexArray(this->vertexArrayObject);

	// Create vertexBuffers
	glGenBuffers(1, &this->vertexArrayBuffer);
	// Fill vertex buffer with (ppp, uv, nnn, ttt)
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	// POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// TEXCOORD
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVCoords));

	// NORMAL
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// TANGENT
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//// BONE ID'S
	//glEnableVertexAttribArray(4);
	//glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneData.IDs));
	//
	//// BONE WEIGHTS
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneData.BoneWeights));

	// Create vertexIndexBuffer
	glGenBuffers(1, &this->vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Bind()
{
	glBindVertexArray(this->vertexArrayObject);
}

void Mesh::BindMaterial(Shader * shader)
{
	this->material->BindMaterial(shader);
}

void Mesh::Draw()
{
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
