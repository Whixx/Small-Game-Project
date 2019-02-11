#include "Object.h"

Object::Object(Mesh *mesh, Texture *texture)
{
	this->mesh = mesh;
	this->texture = texture;
	this->id = -1;
}

Object::Object(Mesh * mesh, Texture * texture, unsigned int id)
{
	this->mesh = mesh;
	this->texture = texture;
	this->id = id;
}

Object::Object()
{
}

Object::~Object()
{
}

glm::mat4 Object::GetWorldMatrix() const
{
	return this->transform.GetWorldMatrix();
}

Transform Object::GetTransform() const
{
	return this->transform;
}

glm::vec3 & Object::GetPos()
{
	return this->transform.GetPos();
}

glm::vec3 & Object::GetRot()
{
	return this->transform.GetRot();
}

glm::vec3 & Object::GetScale()
{
	return this->transform.GetScale();
}

void Object::Update(double dt)
{
}

void Object::Draw()
{
	this->mesh->Draw();
}

void Object::BindTexture()
{
	this->texture->Bind(0);
}
