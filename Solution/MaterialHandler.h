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
	Material* AddMaterial(Texture* tex_diffuse, Texture* tex_ambient, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess = 16, const char* name = "default");
	Material* GetMaterial(string name);

	

private:
	MaterialHandler() {}
	["name"] = Material;
	std::unordered_map<std::string, Material*> materials;
};

#endif