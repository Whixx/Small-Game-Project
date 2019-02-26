#include "MaterialHandler.h"

#include <iostream>

MaterialHandler& MaterialHandler::GetInstance()
{
	static MaterialHandler instance;
	return instance;
}

Material* MaterialHandler::AddMaterial(Texture* tex_diffuse, Texture* tex_ambient, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess, const char* name)
{
	// Check if material exists
	if (this->GetMaterial(name) != nullptr)
		return this->GetMaterial(name);

	// Else create material
	materials[name] = new Material(tex_diffuse, tex_ambient, tex_specular, tex_normal, tex_height, shininess, name);
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
	while(it != this->materials.end())
	{
		delete it->second; // you need to delete object that you created by new before
		it++;
	}
}