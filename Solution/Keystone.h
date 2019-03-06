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

	// Get functions
	bool IsActive();
	glm::vec3 GetWorldPosition();
	Transform * GetTransform();

	void Draw(Model* keyStoneModel, Shader* shader);

	void ActivateKeystone();
private:
	bool isActive;
	glm::vec3 worldPosition;
	Transform transform;
};

#endif