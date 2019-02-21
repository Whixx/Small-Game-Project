#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew\glew.h>
#include <iostream>
#include <vector>
#include "Texture.h"
#include "Shader.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec2 UVCoords;
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

struct Material {
	glm::vec3 specularColor = glm::vec3(1.0, 1.0, 1.0);
	float shininess = 16;
	float d = 0.0f; // Not used
};


class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;
	Material material;

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffer;
	GLuint vertexIndexBuffer;

	void SetupMesh();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures, Material mat);
	virtual ~Mesh();

	void Bind();
	void BindTextures(Shader* shader, unsigned int slot = 0);
	void SendMaterial(Shader* shader);
	void Draw();

	inline std::vector<Texture*> GetTextures() const { return this->textures; };
};
#endif //MESH_H