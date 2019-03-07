#include "Keystone.h"

Keystone::Keystone(KeystonePosDir * keystonePosDir, const int ScaleXZ)
{
	this->transform.GetScale() = glm::vec3(0.125f);	// .25 in diamater

	if (keystonePosDir == nullptr)
	{
		this->transform.SetPos(glm::vec3());
		this->direction = glm::vec3();
	}
	else
	{
		this->transform.SetPos(keystonePosDir->position);
		this->direction = keystonePosDir->direction;
	}

	// Offset to make the cube go abit further in
	float offset = 0.01 * this->transform.GetScale().x;

	// To avoid translating the cube inside displaced walls, we need a tesselation offset to avoid this
	float tesselationOffset = 0.08 * ScaleXZ;

	// The length the cube should be translated
	this->TranslationLength = this->transform.GetScale().x - tesselationOffset + offset;

	this->isActive = false;
	this->isTranslatedBack = false;

	this->movementSpeed = 0.035f;
}

Keystone::~Keystone()
{
}

glm::vec3 Keystone::GetDirection()
{
	return this->direction;
}

float Keystone::GetTranslationLength()
{
	return this->TranslationLength;
}

float Keystone::GetMovementSpeed()
{
	return this->movementSpeed;
}

Transform * Keystone::GetTransform()
{
	return &this->transform;
}

void Keystone::SetIsTranslatedBack(bool isTranslatedBack)
{
	this->isTranslatedBack = isTranslatedBack;
}

void Keystone::UpdateTranslationLength(float lengthMoved)
{
	this->TranslationLength -= lengthMoved;
}

void Keystone::Draw(Model * keyStoneModel, Shader * shader)
{
	keyStoneModel->Draw(shader);
}

void Keystone::ActivateKeystone()
{
	this->isActive = true;
}

bool Keystone::IsActive()
{
	return this->isActive;
}

bool Keystone::IsTranslatedBack()
{
	return this->isTranslatedBack;
}
