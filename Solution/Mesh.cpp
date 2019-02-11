#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, unsigned int numOfVertices)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	positions.reserve(numOfVertices);
	texCoords.reserve(numOfVertices);

	for (unsigned int i = 0; i < numOfVertices; i++)
	{
		positions.push_back(*vertices[i].GetPos());
		texCoords.push_back(*vertices[i].GetTexCoord());

	}

	CreateMesh(positions, texCoords);
}

//Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords)
//{
//	CreateMesh(vertices, uvCoords);
//}

Mesh::Mesh(const char * meshPath)
{
	CreateMesh(meshPath);
}

Mesh::Mesh(const char * meshPath, glm::vec3 color)
{
	CreateMesh(meshPath, color);
}

void Mesh::Draw()
{
	glBindVertexArray(this->vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, this->drawCount);

	glBindVertexArray(0);
}

unsigned int Mesh::GetDrawCount()
{
	return this->drawCount;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->vertexArrayObject);
	glDeleteBuffers(1, this->vertexArrayBuffers);
}
bool Mesh::CreateMesh(const char * meshPath)
{
	bool loaded = false;

	vector<glm::vec3> vertices;
	vector<glm::vec2> uvCoords;
	vector<glm::vec3> normals;

	loaded = LoadMesh(meshPath, vertices, uvCoords, normals);

	drawCount = vertices.size();

	glGenVertexArrays(1, &this->vertexArrayObject);
	glBindVertexArray(this->vertexArrayObject);

	glGenBuffers(NUM_OF_BUFFERS, this->vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(uvCoords[0]), &uvCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	return loaded;
}

void Mesh::CreateMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords)
{
	drawCount = vertices.size();

	glGenVertexArrays(1, &this->vertexArrayObject);
	glBindVertexArray(this->vertexArrayObject);

	glGenBuffers(NUM_OF_BUFFERS, this->vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(uvCoords[0]), &uvCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

bool Mesh::CreateMesh(const char * meshPath, glm::vec3 color)
{
	bool loaded = false;

	vector<glm::vec3> vertices;
	vector<glm::vec2> uvCoords;
	vector<glm::vec3> normals;

	loaded = LoadMesh(meshPath, vertices, uvCoords, normals);

	drawCount = vertices.size();

	vector<glm::vec3> colors;

	for (int i = 0; i < drawCount; i++)
	{
		// Store the color for each vertex of the obj.
		colors.push_back(color);
	}

	glGenVertexArrays(1, &this->vertexArrayObject);
	glBindVertexArray(this->vertexArrayObject);

	glGenBuffers(NUM_OF_BUFFERS, this->vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

													// This is really color and not texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	return loaded;
}



bool Mesh::LoadMesh(const char * meshPath, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals)
{
	bool meshLoaded = false;
	bool endOfFile = false;

	// Vectors to hold the indexes of the faces in the objFile.
	vector<unsigned int> vertexIndices;
	vector<unsigned int> uvIndices;
	vector<unsigned int> normalIndices;

	// Vectors to temporary store the attributes of each face.
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	// Read the filePath and check if we can open it
	FILE * file = fopen(meshPath, "r");
	if (file == NULL)
	{
#ifdef DEBUG
		cout << "[ERROR] - Cannot open the obj File" << std::endl;
#endif // DEBUG
		meshLoaded = false;
	}
	else
	{
		// Read the file and save the data
		while (endOfFile == false)
		{
			char firstWordOfLine[128];
			int wordSize = fscanf(file, "%s", firstWordOfLine);
			if (wordSize == EOF)
			{
				// There is nothing left to be read
				meshLoaded = true;
				endOfFile = true;
			}
			else
			{
				// Find the value of each "v", "vt", "vt" or "f" and store them in the temp vectors
				if (strcmp(firstWordOfLine, "v") == 0)
				{
					glm::vec3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(firstWordOfLine, "vt") == 0)
				{
					glm::vec2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);

					uv.y = 1 - uv.y;
					temp_uvs.push_back(uv);
				}
				else if (strcmp(firstWordOfLine, "vn") == 0)
				{
					glm::vec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(firstWordOfLine, "f") == 0)
				{
					// find all the indices
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int nrOfIndices = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

					if (nrOfIndices != 9)
					{
#ifdef DEBUG
						std::cout << "[ERROR] Wrong file format!" << std::endl;
#endif // DEBUG
						// Our mesh loader is too simple to be able to deal with an obj file like this
						meshLoaded = false;
						break;
					}

					// Store the indices
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
			}
		}

		unsigned int vertexIndex = 0;
		unsigned int uvIndex = 0;
		unsigned int normalIndex = 0;
		glm::vec3 vertex;
		glm::vec2 uv;
		glm::vec3 normal;
		// Now we store the attributes in the i
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			// Find the index
			vertexIndex = vertexIndices[i];
			uvIndex = uvIndices[i];
			normalIndex = normalIndices[i];

			// Find the value of that index	(-1 because all mesh files starts their indexing at 1 instead of 0)
			vertex = temp_vertices[vertexIndex - 1];
			uv = temp_uvs[uvIndex - 1];
			normal = temp_normals[normalIndex - 1];

			// Store the values in the vectors we recieve in the function
			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
	}
	return meshLoaded;
}