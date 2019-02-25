#include "Material.h"

Material::Material(Texture * tex_diffuse, Texture * tex_ambient, Texture * tex_specular, Texture * tex_normal, Texture * tex_height, float shininess, const char * name)
{
	this->diffuse = tex_diffuse;
	this->ambient= tex_ambient;
	this->specular = tex_specular;
	this->normal = tex_normal;
	this->height = tex_height;

	this->shininess = shininess;
	this->name = name;
}

Material::~Material()
{
	delete this->diffuse;
	delete this->ambient;
	delete this->specular;
	delete this->normal;
	delete this->height;
}

void Material::BindMaterial(Shader * shader)
{
	this->SendMaterial(shader);
	this->BindTextures(shader);
}

void Material::SendMaterial(Shader * shader)
{
	shader->SendFloat("shininess", this->shininess);
}

void Material::BindTextures(Shader * shader)
{
	shader->SendInt(this->diffuse->GetType().c_str(), 0);
	this->diffuse->Bind(0);

	shader->SendInt(this->ambient->GetType().c_str(), 1);
	this->ambient->Bind(1);

	shader->SendInt(this->specular->GetType().c_str(), 2);
	this->specular->Bind(2);

	shader->SendInt(this->normal->GetType().c_str(), 3);
	this->normal->Bind(3);

	shader->SendInt(this->height->GetType().c_str(), 4);
	this->height->Bind(4);
}
