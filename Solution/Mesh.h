#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew\glew.h>
#include <iostream>
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "MeshBase.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec2 UVCoords;
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffer;
	GLuint vertexIndexBuffer;

	void SetupMesh();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* mat);
	virtual ~Mesh();

	void Bind();
	void BindMaterial(Shader* shader);
	void Draw();

	inline void SetMaterial(Material* newMat) { this->material = newMat; }
	inline Material* GetMaterial() const { return this->material; }
};
#endif //MESH_H