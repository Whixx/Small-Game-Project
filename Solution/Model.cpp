#include "Model.h"

Model::Model(std::string path, bool gammaCorrection) : gammaCorrection(gammaCorrection)
{
	this->LoadModel(path);
}

Model::~Model()
{
	// TODO: Load all textures as a function
	// TODO: Remove all textures as a function

	// Delete loaded textures
	for (Texture* t : loadedTextures)
	{
		delete t;
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	// aiProcess_Triangulate makes sure that all verticies are connected as triangles
	// aiProcess_GenNormals generate normals if they are missing
	// aiProcess_CalcTangentSpace calculates normals in tangent space
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	this->directoryPath = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	// Go through all meshes and fill array of them
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// Then go through children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh * Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	// Temp variables
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	// Get Vertex Attributes
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// Temp storage
		Vertex vertex;

		// Position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Texture
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.UVCoords = vec;
		}
		else
			vertex.UVCoords = glm::vec2(0.0f, 0.0f);

		// Normal
		if (mesh->mNormals) // assimp fixes this with aiProcess_GenNormals
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);

		if (mesh->mTangents)
		{
			// Tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// Bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
		{
			//TODO: FIX
			// Tangent
			vector.x = 1;
			vector.y = 0;
			vector.z = 0;
			vertex.Tangent = vector;

			// Bitangent
			vector.x = 0;
			vector.y = 0;
			vector.z = 1;
			vertex.Bitangent = vector;
		}

		vertices.push_back(vertex);
	}

	// Get Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Get Material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureDiffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "TextureSpecular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "TextureNormal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture*> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "TextureHeight");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	VertexBufferLayout layout = VertexBufferLayout();
	layout.Push<float>(3); // Position
	layout.Push<float>(2); // UV
	layout.Push<float>(3); // Normal
	layout.Push<float>(3); // Tangent
	layout.Push<float>(3); // Bitangent

	Mesh* temp = new Mesh(vertices, indices, textures, layout);
	return temp;
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < this->loadedTextures.size(); j++)
		{
			// Remove the directory path of the texture
			std::string textureName = this->loadedTextures[j]->GetPath().substr(this->directoryPath.length() + 1);
			// Check if texture is already loaded
			if (std::strcmp(textureName.data(), str.C_Str()) == 0)
			{
				// Push the loaded texture in to the meshs texture
				textures.push_back(this->loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			// If texture hasn't been loaded already, load it
			Texture* texture = new Texture(this->directoryPath + '/' + str.C_Str(), typeName, this->gammaCorrection);
			textures.push_back(texture);
			this->loadedTextures.push_back(texture); // Add to loaded textures
		}
	}
	return textures;
}
