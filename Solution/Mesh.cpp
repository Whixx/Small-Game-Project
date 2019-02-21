#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures, Material material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->material = material;

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

	// TANGNET
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

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

void Mesh::BindTextures(Shader * shader, unsigned int slot)
{
	const unsigned int textureTypes = 5;
	if(slot >= 0 && slot <= 31 - textureTypes)
	{
		unsigned int diffuseNr = 1;
		unsigned int ambientNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (int i = 0; i < this->textures.size(); i++)
		{
			string number;
			string type = textures[i]->GetType();
			if (type == "TextureDiffuse")
				number = to_string(diffuseNr++);
			else if (type == "TextureAmbient")
				number = to_string(ambientNr++);
			else if (type == "TextureSpecular")
				number = to_string(specularNr++);
			else if (type == "TextureNormal")
				number = to_string(normalNr++);
			else if (type == "TextureHeight")
				number = to_string(heightNr++);

			// Only using the first texture of each type
			if (stoi(number) == 1)
			{
				shader->SendInt(type.c_str(), i);
				this->textures[i]->Bind(i);
			}
		}
	}
}

void Mesh::SendMaterial(Shader* shader)
{
	//shader->SendVec3("Material.specColor", this->material.specularColor.x, this->material.specularColor.y, this->material.specularColor.z);
	//shader->SendInt("Material.shininess", this->material.shininess);
	//shader->SendFloat("Material.d", this->material.d);

	shader->SendFloat("shininess", this->material.shininess);
}

void Mesh::Draw()
{
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
