#ifndef KEYSTONE_H
#define KEYSTONE_H

#include "glm/glm.hpp"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

// Used when creating the keystone
struct KeystonePosDir
{
	glm::vec3 position;
	glm::vec3 direction;
};

class Keystone
{
public:
	Keystone();
	Keystone(KeystonePosDir keystonePosDir);
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
	glm::vec3 direction;
	Transform transform;
};

#endif