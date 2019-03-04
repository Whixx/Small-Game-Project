#include "Keystone.h"



Keystone::Keystone()
{
}

Keystone::Keystone(glm::vec3 worldPosition)
{
	this->worldPosition = worldPosition;
}


Keystone::~Keystone()
{
}

glm::vec3 Keystone::GetWorldPosition()
{
	return this->worldPosition;
}
