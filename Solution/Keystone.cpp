#include "Keystone.h"

Keystone::Keystone(KeystonePosDir * keystonePosDir, const int ScaleXZ)
{
	this->transform.GetScale() = glm::vec3(0.73f);

	if (keystonePosDir == nullptr)
	{
		this->transform.SetPos(glm::vec3());
		this->direction = glm::vec3();
	}
	else
	{
		this->transform.SetPos(keystonePosDir->position);
		this->direction = keystonePosDir->direction;
		this->SetDir(this->direction);
	}

	// To avoid translating the cube inside displaced walls, we need a tesselation offset to avoid this
	float tesselationOffset = 0.03 * ScaleXZ;

	// The length the cube should be translated
	this->TranslationLength = -tesselationOffset + 0.10;

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

void Keystone::SetDir(glm::vec3 newDir)
{
	newDir = glm::normalize(newDir);
	float angle = glm::acos(-newDir.x);

	// Check if under the unit circle
	if (glm::sin(newDir.z) < 0.0)
	{
		angle *= -1;
	}

	this->transform.GetRot().y = angle;
}

bool Keystone::IsActive()
{
	return this->isActive;
}

bool Keystone::IsTranslatedBack()
{
	return this->isTranslatedBack;
}
