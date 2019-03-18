#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include "MaterialHandler.h"
#include "AnimatedMesh.h"
#include "AnimatedSkeleton.h"

using namespace std;

class Model
{
public:
	Model(std::string path = "Models/Cube/cube.obj", bool gammaCorrection = false);
	virtual ~Model();

	inline std::vector<Mesh*> GetMeshes() const { return this->meshes; };
	inline std::string GetDirectory() const { return this->directoryPath; };
	void SetMaterial(Material* newMat);

	void Draw(Shader* shader);
	void DrawMeshes(Shader* shader);

private:
	bool gammaCorrection;
	std::vector<Mesh*> meshes;
	std::string directoryPath;
	std::string name;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	std::vector<Texture*> LoadNormalMap(std::string path, std::string type);
};


class AnimatedModel
{
public:
	AnimatedModel(std::string path = "Models/Cube/cube.obj", bool gammaCorrection = false);
	virtual ~AnimatedModel();

	inline std::string GetDirectory() const { return this->directoryPath; };
	const SkeletonBuffer& GetSkeletonBuffer();
	void SetMaterial(Material* newMat);
	void init();

	void Update(double dt);
	void Draw(Shader* shader);
	void DrawMeshes(Shader* shader);

private:
	bool gammaCorrection;
	std::string directoryPath;
	std::string name;
	AnimatedMesh* mesh;
	AnimatedSkeleton* skeleton;

	std::vector<Texture*> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	std::vector<Texture*> LoadNormalMap(std::string path, std::string type);
};

#endif //MODEL_H