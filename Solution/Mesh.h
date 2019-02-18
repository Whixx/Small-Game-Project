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
	glm::vec3 Bitangent;
};


class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffer;
	GLuint vertexIndexBuffer;

	void SetupMesh();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
	virtual ~Mesh();

	void Bind();
	void BindTextures(Shader* shader, unsigned int slot = 0);
	void Draw();

	inline std::vector<Texture*> GetTextures() const { return this->textures; };
};

/*

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numOfVertices);
	//Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords);
	Mesh(const char * meshPath);
	Mesh(const char * meshPath, glm::vec3 color);
	Mesh() {}
	void Draw();
	unsigned int GetDrawCount();

	void operator=(const Mesh& other) {}

	virtual ~Mesh();
	bool CreateMesh(const char * meshPath);
	void CreateMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords);
	bool CreateMesh(const char * meshPath, glm::vec3 color);
	bool LoadMesh(const char * objectPath, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals);

private:
};


*/
#endif //MESH_H