//#include <EnginePch.h>
#include "AnimatedMesh.h"

std::vector<Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const char* path);


AnimatedMesh::AnimatedMesh(
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
		std::vector<MeshEntry>& entries,
		Material* mat) noexcept
	: m_VAO(0),
	m_VBO(0),
	m_IBO(0),
	m_BonesBuffer(0),
	m_InstanceBuffer(0),
	m_NumVerts(0),
	m_NumIndices(0),
	m_GlobalInverseTransform(0.0f),
	m_pVertices(nullptr),
	m_pBones(nullptr),
	m_pIndices(nullptr),
	m_pRootNode(nullptr),
	m_pScene(nullptr),
	m_NumBones(0)
{
	//Copy data for use in construct
	m_pVertices = new BaseVertex[numVertices];
	memcpy(m_pVertices, pVertices, sizeof(BaseVertex) * numVertices);
	m_pIndices = new unsigned int[numIndices];
	memcpy(m_pIndices, pIndices, sizeof(unsigned int) * numIndices);
	m_pBones = new VertexBoneData[numVertices];
	memcpy(m_pBones, pBones, sizeof(VertexBoneData) * numVertices);

	m_pImporter = pImporter;
	m_NumVerts = numVertices;
	m_NumIndices = numIndices;
	m_NumBones = numBones;
	m_GlobalInverseTransform = globalTransform;
	m_BoneOffsets = boneOffsets;
	m_BoneMap = boneMap;
	m_pRootNode = pRootNode;
	m_pScene = pScene;
	m_Entries = entries;
	this->material = mat;
}

AnimatedMesh::~AnimatedMesh()
{
	const Assimp::Importer* pImporter = reinterpret_cast<const Assimp::Importer*>(m_pImporter);
	DeleteSafe(pImporter);
}

void AnimatedMesh::Draw()
{
	glDrawElements(GL_TRIANGLES, this->m_NumVerts, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void AnimatedMesh::Bind()
{
	glBindVertexArray(this->m_VAO);
}

void AnimatedMesh::BindMaterial(Shader* shader)
{
	this->material->BindMaterial(shader);
}

AnimatedMesh* AnimatedMesh::ReadColladaFile(const char* pFilename)
{
	Assimp::Importer* importer = new Assimp::Importer();
	const aiScene* pScene = nullptr;
	const aiNode* pRootNode = nullptr;
	VertexBoneData* pBones = nullptr;
	unsigned int nrOfVerts = 0;
	unsigned int nrOfbones = 0;
	unsigned int nrOfIndices = 0;
	glm::mat4 globalTransform(0.0);
	std::map<std::string, unsigned int> boneMap;
	std::vector<glm::mat4> boneOffsets;
	std::vector<MeshEntry> entries;

	/*
#if !defined(DEBUG)
	pScene = importer->ReadFile(pFilename, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_LimitBoneWeights | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#else
	pScene = importer->ReadFile(pFilename, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_LimitBoneWeights | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#endif
*/

	pScene = importer->ReadFile(pFilename, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_LimitBoneWeights);
	if (!pScene)
	{
		std::cout << "File couldn't be loaded" << std::endl;
		return nullptr;
	}
	else
	{
		pRootNode = pScene->mRootNode;

		if (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::cout << "Error: Scene incomplete." << std::endl;
			return nullptr;
		}

		if (!pScene->HasMeshes())
		{
			std::cout << "Error: Scene does not contain any meshes." << std::endl;
			return nullptr;
		}
		else
		{
			std::cout << "Scene has " << pScene->mNumMeshes << " meshes." << std::endl;
		}

		glm::mat4 globalTransform = AssimpToGLMMat4(pScene->mRootNode->mTransformation);
		globalTransform = glm::inverse(globalTransform);

		const aiMesh* pMesh = pScene->mMeshes[0];
		if (!pMesh->HasBones())
		{
			std::cout << "Error: Mesh does not contain any bones." << std::endl;
			return nullptr;
		}
		else
		{
			std::cout << "Scene has " << pMesh->mNumBones << " bones." << std::endl;
		}


		if (pScene->mRootNode->mMetaData)
		{
			aiMetadata* pMetaData = pScene->mRootNode->mMetaData;
			std::cout << "Metadata: " << std::endl;
			for (unsigned int i = 0; i < pMetaData->mNumProperties; i++)
			{
				std::cout << pMetaData->mKeys->C_Str() << std::endl;
			}
		}
		else
		{
			std::cout << "No metadata" << std::endl;
		}
		
		const aiBone* paiBones = pMesh->mBones[0];

		std::vector<MeshEntry> entries;
		entries.resize(pScene->mNumMeshes);
		std::cout << "Number of Meshes in file: " << entries.size() << std::endl;

		//making space for the vertex attributes and indices.
		std::vector<aiMesh*> paiMeshes;

		std::cout << "Loading data..." << std::endl;
		//counting the number of indices and vertices in mesh
		for (unsigned int i = 0; i < entries.size(); i++)
		{
			entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			entries[i].BaseVertex = nrOfVerts;
			entries[i].BaseIndex = nrOfIndices;

			nrOfVerts += pScene->mMeshes[i]->mNumVertices;
			nrOfIndices += entries[i].NumIndices;

			//load all the meshes in to a vector array. 
			paiMeshes.push_back(pScene->mMeshes[i]);
		}

		//Extracting all the needed data from the mesh/meshes
		std::vector<BaseVertex> verts;
		std::vector<unsigned int> indices;
		pBones = new VertexBoneData[nrOfVerts];
		Material* mat = nullptr;
		for (unsigned int i = 0; i < entries.size(); i++)
		{
			for (unsigned int j = 0; j < paiMeshes.at(i)->mNumVertices; j++)
			{
				BaseVertex data = {};
				data.Position = glm::vec3(paiMeshes.at(i)->mVertices[j].x, paiMeshes.at(i)->mVertices[j].y, paiMeshes.at(i)->mVertices[j].z);
				data.Normal = (paiMeshes.at(i)->HasNormals()) ? glm::vec3(paiMeshes.at(i)->mNormals[j].x, paiMeshes.at(i)->mNormals[j].y, paiMeshes.at(i)->mNormals[j].z) : glm::vec3();
				data.TexCoord = (paiMeshes.at(i)->HasTextureCoords(0)) ? glm::vec2(paiMeshes.at(i)->mTextureCoords[0][j].x, paiMeshes.at(i)->mTextureCoords[0][j].y) : glm::vec2();
				data.Tangent = glm::vec3(paiMeshes.at(i)->mTangents[j].x, paiMeshes.at(i)->mTangents[j].y, paiMeshes.at(i)->mTangents[j].z);
				verts.push_back(data);
			}

			int index = 0;
			for (unsigned int f = 0; f < paiMeshes.at(i)->mNumFaces; f++)
			{
				const aiFace& Face = paiMeshes.at(i)->mFaces[f];
				if (Face.mNumIndices == 3)
				{
					indices.push_back(Face.mIndices[0]);
					indices.push_back(Face.mIndices[1]);
					indices.push_back(Face.mIndices[2]);
				}
			}

			//loop through bones
			for (unsigned int b = 0; b < paiMeshes.at(i)->mNumBones; b++)
			{
				if (b >= MAX_NUM_BONES)
				{
					std::cout << "Number of bones exceeds limit of " << MAX_NUM_BONES << " bones." << std::endl;
					break;
				}

				unsigned int boneIndex = 0;
				std::string boneName(paiMeshes.at(i)->mBones[b]->mName.data);
				std::cout << "extracting data from bone: " << paiMeshes.at(i)->mBones[b]->mName.data << std::endl;

				if (boneMap.find(boneName) == boneMap.end())
				{
					//allocate an index for a new bone
					glm::mat4 boneOffset;
					boneIndex = nrOfbones;
					nrOfbones++;
					boneOffsets.push_back(boneOffset);
					boneOffsets[boneIndex] = AssimpToGLMMat4(pMesh->mBones[b]->mOffsetMatrix); //function that transforms assimp matrix to glm matrix
					boneMap[boneName] = boneIndex;
				}
				else
				{
					boneIndex = boneMap[boneName];
				}

				//loop through weights
				for (unsigned int w = 0; w < paiMeshes.at(i)->mBones[b]->mNumWeights; w++)
				{
					unsigned int vertexID = entries[i].BaseVertex + paiMeshes.at(i)->mBones[b]->mWeights[w].mVertexId;
					float weight = paiMeshes.at(i)->mBones[b]->mWeights[w].mWeight;
					pBones[vertexID].AddBoneData(boneIndex, weight);
				}
			}


			aiString matName;
			float matShininess;
			aiColor3D matAmbient;
			MaterialHandler& MH = MaterialHandler::GetInstance();
			std::vector<Texture*> diffuseMaps;
			std::vector<Texture*> emissiveMaps;
			std::vector<Texture*> specularMaps;
			std::vector<Texture*> normalMaps;
			std::vector<Texture*> heightMaps;

			// Get Material
			if (paiMeshes.at(i)->mMaterialIndex >= 0)
			{
				aiMaterial *material = pScene->mMaterials[paiMeshes.at(i)->mMaterialIndex];

				unsigned int success;
				// Load material variables
				success = material->Get(AI_MATKEY_NAME, matName);
				if (success != AI_SUCCESS)
				{
				}

				success = material->Get(AI_MATKEY_SHININESS, matShininess);
				if (success != AI_SUCCESS)
				{
				}

				success = material->Get(AI_MATKEY_COLOR_AMBIENT, matAmbient);
				if (success != AI_SUCCESS)
				{
				}

				// Load all texture types
				diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureDiffuse", pFilename);
				emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "TextureEmissive", pFilename);
				specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "TextureSpecular", pFilename);
				normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "TextureNormal", pFilename);
				heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "TextureHeight", pFilename);

				// Add default textures if nothing was loaded
				if (diffuseMaps.size() == 0)
					diffuseMaps.push_back(MH.LoadTexture("Textures/default/default_diffuse.png", "TextureDiffuse"));
				if (emissiveMaps.size() == 0)
					emissiveMaps.push_back(MH.LoadTexture("Textures/default/default_emissive.png", "TextureEmissive"));
				if (specularMaps.size() == 0)
					specularMaps.push_back(MH.LoadTexture("Textures/default/default_specular.png", "TextureSpecular"));
				if (normalMaps.size() == 0)
					normalMaps.push_back(MH.LoadTexture("Textures/default/default_normal.png", "TextureNormal"));
				if (heightMaps.size() == 0)
					heightMaps.push_back(MH.LoadTexture("Textures/default/default_height.png", "TextureHeight"));
			}
			else
			{
				diffuseMaps.push_back(MH.LoadTexture("Textures/default/default_diffuse.png", "TextureDiffuse"));
				emissiveMaps.push_back(MH.LoadTexture("Textures/default/default_emissive.png", "TextureEmissive"));
				specularMaps.push_back(MH.LoadTexture("Textures/default/default_specular.png", "TextureSpecular"));
				normalMaps.push_back(MH.LoadTexture("Textures/default/default_normal.png", "TextureNormal"));
				heightMaps.push_back(MH.LoadTexture("Textures/default/default_height.png", "TextureHeight"));
			}

			mat = MH.AddMaterial(diffuseMaps[0], emissiveMaps[0], specularMaps[0], normalMaps[0], heightMaps[0], matShininess, glm::vec3(matAmbient.r, matAmbient.g, matAmbient.b), matName.C_Str());

		}



		AnimatedMesh* pResult = new AnimatedMesh(verts.data(), indices.data(), pBones, nrOfVerts, nrOfIndices, nrOfbones, globalTransform, boneMap, boneOffsets, pRootNode, pScene, importer, entries, mat);
		DeleteArrSafe(pBones);

		return pResult;
	}
}

std::vector<Texture*> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, const char* path)
{
	std::string tempPath = std::string(path);
	std::string directory = tempPath.substr(0, tempPath.find_last_of('/'));

	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str); // Get texture name
		textures.push_back(MaterialHandler::GetInstance().LoadTexture((directory + '/' + str.C_Str()).c_str(), typeName));
	}
	return textures;
}

void AnimatedMesh::Construct()
{

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenBuffers(1, &m_BonesBuffer);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(BaseVertex), m_pVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(unsigned int), m_pIndices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)0);
	glEnableVertexAttribArray(0);
	
	//TexCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(BaseVertex), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TANGENT
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(BaseVertex), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	//Bone data that will be sent to shader for calculations.
	glBindBuffer(GL_ARRAY_BUFFER, m_BonesBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(VertexBoneData), m_pBones, GL_STATIC_DRAW);

	//BoneID
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (void*)0);
	glEnableVertexAttribArray(4);
	//BoneWeight
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)16);
	glEnableVertexAttribArray(5);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	DeleteArrSafe(m_pVertices);
	DeleteArrSafe(m_pIndices);
	DeleteArrSafe(m_pBones);
}