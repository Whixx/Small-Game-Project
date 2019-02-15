#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew\glew.h>
#include <iostream>
#include <vector>

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
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,
		BITANGENT_VB,

		NUM_OF_BUFFERS
	};

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture*> m_Textures;

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_OF_BUFFERS];
	unsigned int drawCount;

	void SetupMesh();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures, VertexBufferLayout layout);
	virtual ~Mesh();

	void Bind();

	inline std::vector<Texture*> GetTextures() const { return m_Textures; };
	inline unsigned int GetCount() const { return IB->GetCount(); };
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