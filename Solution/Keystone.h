#ifndef KEYSTONE_H
#define KEYSTONE_H

#include "glm/glm.hpp"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

class Keystone
{
public:
	Keystone();
	Keystone(glm::vec3 worldPosition);
	~Keystone();

	glm::vec3 GetWorldPosition();
	Transform * GetTransform();

	void Draw(Model* keyStoneModel, Shader* shader);

	void ActivateKeystone();

	// Get functions
	bool IsActive();
private:
	glm::vec3 worldPosition;

	Transform transform;

	bool isActive;
};

#endif