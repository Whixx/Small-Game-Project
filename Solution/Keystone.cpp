#include "Keystone.h"



Keystone::Keystone()
{
}

Keystone::Keystone(glm::vec3 worldPosition)
{
	this->worldPosition = worldPosition;
	this->isActive = false;
}


Keystone::~Keystone()
{
}

glm::vec3 Keystone::GetWorldPosition()
{
	return this->worldPosition;
}

void Keystone::ActivateKeystone()
{
	this->isActive = true;
}

bool Keystone::IsActive()
{
	return this->isActive;
}
