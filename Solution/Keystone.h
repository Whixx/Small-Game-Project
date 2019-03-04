#ifndef KEYSTONE_H
#define KEYSTONE_H

#include "glm/glm.hpp"

class Keystone
{
public:
	Keystone();
	Keystone(glm::vec3 worldPosition);
	~Keystone();

	glm::vec3 GetWorldPosition();
private:
	glm::vec3 worldPosition;
};

#endif