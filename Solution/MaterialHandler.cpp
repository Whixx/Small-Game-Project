#include "MaterialHandler.h"

#include <iostream>

MaterialHandler& MaterialHandler::GetInstance()
{
	static MaterialHandler instance;
	return instance;
}

Material* MaterialHandler::AddMaterial(Texture* tex_diffuse, Texture* tex_emissive, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess, glm::vec3 ambient, const char* name)
{
	// Check if material exists
	if (this->GetMaterial(name) != nullptr)
	{
		cout << "[WARNING]: Multiple material names detected, some objects might look wierd" << endl;
		return this->GetMaterial(name);
	}

	// If nullptrs use default textures
	if (tex_diffuse == nullptr)
	{
		tex_diffuse = this->default_diffuse;
	}
	if (tex_emissive == nullptr)
	{
		tex_emissive = this->default_emissive;
	}
	if (tex_specular == nullptr)
	{
		tex_specular = this->default_specular;
	}
	if (tex_normal == nullptr)
	{
		tex_normal = this->default_normal;
	}
	if (tex_height == nullptr)
	{
		tex_height = this->default_height;
	}

	// Else create material
	materials[name] = new Material(tex_diffuse, tex_emissive, tex_specular, tex_normal, tex_height, shininess, ambient, name);
	return materials[name];
}

Material* MaterialHandler::GetMaterial(string name)
{
	if (materials.find(name) != materials.end())
		return materials[name];
	else
		return nullptr;
}

Texture* MaterialHandler::LoadTexture(string path, string type)
{
	// Check if loaded already
	for (unsigned int j = 0; j < this->loadedTextures.size(); j++)
	{
		// Remove the directory path of the texture
		string loadedTexturePath = this->loadedTextures[j]->GetPath();
		// Check if texture is already loaded
		if (std::strcmp(loadedTexturePath.c_str(), path.c_str()) == 0)
		{
			// Push the loaded texture in to the meshs texture
			return this->loadedTextures[j];
		}
	}
	
	// If texture hasn't been loaded already, load it
	this->loadedTextures.push_back(new Texture(path, type)); // Add to loaded textures
	return this->loadedTextures[this->loadedTextures.size()-1];
}

MaterialHandler::~MaterialHandler()
{
	// Free up Textures
	for (int i = 0; i < this->loadedTextures.size(); i++)
	{
		delete this->loadedTextures[i];
	}

	auto it = this->materials.begin();
	while (it != this->materials.end())
	{
		delete it->second; // you need to delete object that you created by new before
		it++;
	}
}

void MaterialHandler::LoadDefaultTextures()
{
	this->default_diffuse = this->LoadTexture("Textures/default/default_diffuse.png", "TextureDiffuse");
	this->default_normal = this->LoadTexture("Textures/default/default_normal.png", "TextureNormal");
	this->default_specular = this->LoadTexture("Textures/default/default_specular.png", "TextureSpecular");
	this->default_emissive = this->LoadTexture("Textures/default/default_emissive.png", "TextureEmissive");
	this->default_height = this->LoadTexture("Textures/default/default_height.png", "TextureHeight");
}