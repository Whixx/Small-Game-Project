#include "Material.h"

Material::Material(Texture * tex_diffuse, Texture * tex_emissive, Texture * tex_specular, Texture * tex_normal, Texture * tex_height, float shininess, glm::vec3 ambient, const char * name)
{
	this->diffuse = tex_diffuse;
	this->emissive = tex_emissive;
	this->specular = tex_specular;
	this->normal = tex_normal;
	this->height = tex_height;

	this->shininess = shininess;
	this->ambient = ambient;
	this->name = name;
}

Material::~Material()
{
	
}

void Material::BindMaterial(Shader * shader)
{
	this->SendMaterial(shader);
	this->BindTextures(shader);
}

void Material::BindMaterialArray(Shader* shader, unsigned int type)
{
	// 5 Textures before next type
	unsigned int stride = 5;

	// Textures
	shader->SendInt((this->diffuse->GetType() + '[' + to_string(type) + ']').c_str(), type * stride);
	this->diffuse->Bind(type * stride);

	shader->SendInt((this->emissive->GetType() + '[' + to_string(type) + ']').c_str(), type * stride + 1);
	this->emissive->Bind(type * stride + 1);

	shader->SendInt((this->specular->GetType() + '[' + to_string(type) + ']').c_str(), type * stride + 2);
	this->specular->Bind(type * stride + 2);

	shader->SendInt((this->normal->GetType() + '[' + to_string(type) + ']').c_str(), type * stride + 3);
	this->normal->Bind(type * stride + 3);

	shader->SendInt((this->height->GetType() + '[' + to_string(type) + ']').c_str(), type * stride + 4);
	this->height->Bind(type * stride + 4);

	// Attributes
	shader->SendFloat((string("shininess") + '[' + to_string(type) + ']').c_str(), this->shininess);

	shader->SendVec3((string("ambient") + '[' + to_string(type) + ']').c_str(), this->ambient.r, this->ambient.g, this->ambient.b);
}

void Material::SendMaterial(Shader * shader)
{
	shader->SendFloat("shininess", this->shininess);
	shader->SendVec3("ambient", this->ambient.r, this->ambient.g, this->ambient.b);
}

void Material::BindTextures(Shader * shader)
{
	shader->SendInt(this->diffuse->GetType().c_str(), 0);
	this->diffuse->Bind(0);

	shader->SendInt(this->emissive->GetType().c_str(), 1);
	this->emissive->Bind(1);

	shader->SendInt(this->specular->GetType().c_str(), 2);
	this->specular->Bind(2);

	shader->SendInt(this->normal->GetType().c_str(), 3);
	this->normal->Bind(3);

	shader->SendInt(this->height->GetType().c_str(), 4);
	this->height->Bind(4);
}
