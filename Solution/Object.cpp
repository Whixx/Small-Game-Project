#include "Object.h"

Object::Object(string path)
{
	this->model = new Model(path, false);
}

Object::~Object()
{
	delete this->model;
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

void Object::Draw(Shader* shader)
{
	this->model->Draw(shader);
}

Model * Object::GetModel() const
{
	return this->model;
}
