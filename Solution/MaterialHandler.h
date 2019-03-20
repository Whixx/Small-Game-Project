#ifndef MATERIALHANDLER_H
#define MATERIALHANDLER_H

#include "Material.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class MaterialHandler
{
public:	
	MaterialHandler(const MaterialHandler& other) = delete;
	MaterialHandler& operator=(const MaterialHandler& other) = delete;

	static MaterialHandler& GetInstance();
	Material* AddMaterial(Texture* tex_diffuse, Texture* tex_emissive, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess, glm::vec3 ambient, const char* name);
	Material* GetMaterial(string name);

	Texture* LoadTexture(string path, string type);

private:
	MaterialHandler() { this->LoadDefaultTextures(); }
	~MaterialHandler();
	std::unordered_map<std::string, Material*> materials;
	std::vector<Texture*> loadedTextures;

	Texture* default_diffuse;
	Texture* default_specular;
	Texture* default_normal;
	Texture* default_emissive;
	Texture* default_height;

	void LoadDefaultTextures();
};

#endif