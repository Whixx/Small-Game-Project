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
	//Keystone();
	Keystone(KeystonePosDir * keystonePosDir = nullptr, const int ScaleXZ = 1);
	~Keystone();

	// Get functions
	bool IsActive();
	bool IsTranslatedBack();
	glm::vec3 GetDirection();
	float GetTranslationLength();
	float GetMovementSpeed();
	Transform * GetTransform();

	// Set functions
	void SetIsTranslatedBack(bool isTranslatedBack);
	void UpdateTranslationLength(float lengthMoved);

	void Draw(Model* keyStoneModel, Shader* shader);

	void ActivateKeystone();
private:
	
	Transform transform;

	glm::vec3 direction;
	float TranslationLength;

	bool isActive;
	bool isTranslatedBack;

	float movementSpeed;
};

#endif