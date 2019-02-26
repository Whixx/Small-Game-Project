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
	materials[name] = Material(tex_diffuse, tex_ambient, tex_specular, tex_normal, tex_height, shininess, name);
	return &materials[name];
}

Material* MaterialHandler::GetMaterial(string name)
{
	if (materials.find(name) != materials.end())
		return &materials[name];
	else
		return nullptr;
}