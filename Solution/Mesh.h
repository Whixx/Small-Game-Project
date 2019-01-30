#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew\glew.h>
#include <iostream>
#include <vector>

using namespace std;

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
	}

	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec2* GetTexCoord() { return &texCoord; }
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
};

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
	bool createMesh(const char * meshPath);
	void createMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords);
	bool createMesh(const char * meshPath, glm::vec3 color);
	bool loadMesh(const char * objectPath, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals);
private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,

		NUM_OF_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_OF_BUFFERS];
	unsigned int drawCount;
};

#endif //MESH_H