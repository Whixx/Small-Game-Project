#pragma once
//#include <EnginePch.h>

#include <glm/glm.hpp>

const unsigned int MAX_NUM_BONES_PER_VERT = 4;

//Base vertex
struct BaseVertex
{
public:
	BaseVertex(glm::vec3 position = glm::vec3(0.0f), glm::vec3 normal = glm::vec3(0.0f), glm::vec2 texCoord = glm::vec2(0.0f), glm::vec3 tangent = glm::vec3(0.0f));
	
	bool operator==(const BaseVertex& other);
	bool operator!=(const BaseVertex& other);

public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

inline BaseVertex::BaseVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec3 tangent)
{
	Position = position;
	Normal = normal;
	TexCoord = texCoord;
	Tangent = tangent;
}

inline bool BaseVertex::operator==(const BaseVertex& other)
{
	return (Position == other.Position) && (Normal == other.Normal) && (TexCoord == other.TexCoord);
}

inline bool BaseVertex::operator!=(const BaseVertex& other)
{
	return !(*this == other);
}

//Instance
struct InstanceData
{
	glm::mat4 Model;
};

//Bonedata
struct VertexBoneData
{
public:
	glm::ivec4 IDs = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);

public:
	void AddBoneData(unsigned int BoneID, float Weight);
};

inline void VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < MAX_NUM_BONES_PER_VERT; i++)
	{
		if (BoneWeights[i] == 0.0f)
		{
			IDs[i] = BoneID;
			BoneWeights[i] = Weight;
			return;
		}
	}
}