#include "Object.h"

Object::Object(Mesh *mesh, Transform transform, Texture *texture, unsigned int m_id)
{
	this->mesh = mesh;
	this->transform = transform;
	this->texture = texture;
	this->m_id = m_id;
}

Object::Object()
{
}

Object::~Object()
{

}

glm::mat4 Object::getWorldMatrix() const
{
	return this->transform.getWorldMatrix();
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

void Object::Draw()
{
	this->mesh->Draw();
}

void Object::bindTexture()
{
	this->texture->Bind(0);
}
