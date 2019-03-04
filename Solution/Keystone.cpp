#include "Keystone.h"



Keystone::Keystone()
{
}

Keystone::Keystone(glm::vec3 worldPosition)
{
	this->worldPosition = worldPosition;
	this->transform.GetScale() = glm::vec3(5.0, 5.0, 5.0);
}


Keystone::~Keystone()
{
}

glm::vec3 Keystone::GetWorldPosition()
{
	return this->worldPosition;
}

Transform * Keystone::GetTransform()
{
	return &this->transform;
}

void Keystone::Draw(Model * keyStoneModel, Shader * shader)
{
	keyStoneModel->Draw(shader);
}
