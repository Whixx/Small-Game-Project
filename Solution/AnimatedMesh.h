#pragma once
//#include <EnginePch.h>
//#include <IO/IResource.h>
//#include <Graphics/Buffers/UniformBuffer.h>
#include "MeshBase.h"

const unsigned int MAX_NUM_BONES = 50;

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glew\glew.h>
#include "AssimpHelper.h"

class AnimatedMesh// : public IResource
{
	//friend class GLContext;
	//friend class MESH;
	//friend class ResourceHandler;
	friend class AnimatedSkeleton;

private:
	struct MeshEntry
	{
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = 0xFFFFFFFF;
		}

		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

public:
	AnimatedMesh(
		BaseVertex* pVertices,
		unsigned int* pIndices,
		VertexBoneData* pBones,
		unsigned int numVertices,
		unsigned int numIndices,
		unsigned int numBones,
		glm::mat4& globalTransform,
		std::map<std::string, unsigned int>& boneMap,
		std::vector<glm::mat4>& boneOffsets,
		const void* pRootNode,
		const void* pScene,
		const void* pImporter,
		std::vector<MeshEntry>& entries) noexcept;

	~AnimatedMesh();

	void Draw();
	void Bind();
	void BindMaterial();
	void Construct();
	static AnimatedMesh* ReadColladaFile(const char* daeFile);
private:

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_BonesBuffer;
	unsigned int m_InstanceBuffer;
	unsigned int m_Texture;
	unsigned int m_NumOfJoints;
	unsigned int m_BoneLocation[MAX_NUM_BONES];
	const void* m_pScene;
	const void* m_pRootNode;
	const void* m_pImporter;
	glm::mat4 m_GlobalInverseTransform;
	BaseVertex* m_pVertices;
	unsigned int m_NumVerts;
	VertexBoneData* m_pBones;
	unsigned int m_NumBones;
	unsigned int* m_pIndices;
	unsigned int m_NumIndices;
	std::vector<MeshEntry> m_Entries;
	mutable std::map<std::string, unsigned int> m_BoneMap;
	mutable std::vector<glm::mat4> m_BoneOffsets;
};