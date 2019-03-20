#include "Model.h"

Model::Model(std::string path, bool gammaCorrection) 
	: gammaCorrection(gammaCorrection)
{
	this->directoryPath = path.substr(0, path.find_last_of('/'));
	std::string objPath = path.substr(path.find_last_of('/') + 1, string::npos); // Remove directory
	this->name = objPath.erase(objPath.find_last_of('.'), string::npos);
	this->LoadModel(path);
}

Model::~Model()
{
	// Delete all the meshes
	for (Mesh* m : this->meshes)
	{
		delete m;
	}
}

void Model::SetMaterial(Material* newMat)
{
	for (Mesh* m : this->meshes)
	{
		m->SetMaterial(newMat);
	}
}

void Model::Draw(Shader* shader)
{
	for (Mesh* m : this->meshes)
	{
		m->BindMaterial(shader);
		m->Bind();
		m->Draw();
	}
}

void Model::DrawMeshes(Shader * shader)
{
	for (Mesh* m : this->meshes)
	{
		m->Bind();
		m->Draw();
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	// aiProcess_Triangulate makes sure that all verticies are connected as triangles
	// aiProcess_GenNormals generate normals if they are missing
	// aiProcess_CalcTangentSpace calculates normals in tangent space
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

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

		// UV
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
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// Tangent IF crash here, probably dont got uv coords on model
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vertices.push_back(vertex);
	}

	// Get Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
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
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

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
		diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureDiffuse");
		emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "TextureEmissive");
		specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "TextureSpecular");
		normalMaps = LoadNormalMap(this->directoryPath + '/' + this->name + "_normal.png", "TextureNormal");
		heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "TextureHeight");

		// Add default textures if nothing was loaded
		if (diffuseMaps.size() == 0)
			diffuseMaps.push_back(nullptr);
		if (emissiveMaps.size() == 0)
			emissiveMaps.push_back(nullptr);
		if (specularMaps.size() == 0)
			specularMaps.push_back(nullptr);
		if (normalMaps.size() == 0)
			normalMaps.push_back(nullptr);
		if (heightMaps.size() == 0)
			heightMaps.push_back(nullptr);
	}
	else
	{
		diffuseMaps.push_back(nullptr);
		emissiveMaps.push_back(nullptr);
		specularMaps.push_back(nullptr);
		normalMaps.push_back(nullptr);
		heightMaps.push_back(nullptr);
	}

	Material* mat = MH.AddMaterial(diffuseMaps[0], emissiveMaps[0], specularMaps[0], normalMaps[0], heightMaps[0], matShininess, glm::vec3(matAmbient.r, matAmbient.g, matAmbient.b), matName.C_Str());
	Mesh* temp = new Mesh(vertices, indices, mat);
	return temp;
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str); // Get texture name
		textures.push_back(MaterialHandler::GetInstance().LoadTexture((this->directoryPath + '/' + str.C_Str()).c_str(), typeName));
	}
	return textures;
}

std::vector<Texture*> Model::LoadNormalMap(std::string path, std::string type)
{
	std::vector<Texture*> normalMaps;
	normalMaps.push_back(MaterialHandler::GetInstance().LoadTexture(path.c_str(), type));

	return normalMaps;
}






AnimatedModel::AnimatedModel(std::string path, bool gammaCorrection) 
	: gammaCorrection(gammaCorrection)
{
	this->directoryPath = path.substr(0, path.find_last_of('/'));
	std::string objPath = path.substr(path.find_last_of('/') + 1, string::npos); // Remove directory
	this->name = objPath.erase(objPath.find_last_of('.'), string::npos);
	this->mesh = AnimatedMesh::ReadColladaFile(path.c_str());
	this->skeleton = new AnimatedSkeleton();
	//this->skeleton->UpdateBoneTransforms(0.0f, this->mesh);
}

AnimatedModel::~AnimatedModel()
{
	// Delete the mesh?
	delete this->mesh;
	delete this->skeleton;
}

const SkeletonBuffer& AnimatedModel::GetSkeletonBuffer()
{
	return this->skeleton->GetSkeletonBuffer();
}

void AnimatedModel::init()
{
	this->mesh->Construct();
}

const glm::mat4 & AnimatedModel::GetBoneTransform(unsigned int index)
{
	return this->skeleton->GetBoneTransform(index);
}

void AnimatedModel::Update(double dt)
{
	this->skeleton->UpdateBoneTransforms(dt, this->mesh);
}

void AnimatedModel::Draw(Shader * shader)
{
	this->mesh->BindMaterial(shader);
	this->mesh->Bind();
	this->mesh->Draw();
}

void AnimatedModel::DrawMeshes(Shader * shader)
{
	this->mesh->Bind();
	this->mesh->Draw();
}

std::vector<Texture*> AnimatedModel::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str); // Get texture name
		textures.push_back(MaterialHandler::GetInstance().LoadTexture((this->directoryPath + '/' + str.C_Str()).c_str(), typeName));
	}
	return textures;
}

std::vector<Texture*> AnimatedModel::LoadNormalMap(std::string path, std::string type)
{
	std::vector<Texture*> normalMaps;
	normalMaps.push_back(MaterialHandler::GetInstance().LoadTexture(path.c_str(), type));

	return normalMaps;
}