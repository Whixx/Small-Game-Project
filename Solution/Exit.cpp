#include "Exit.h"

Exit::Exit(Model* exitModel, glm::vec3 worldPos, glm::vec3 dir, glm::vec2 uvPos)
{
	this->model = exitModel;
	this->uvPos = uvPos;
	this->transform.GetPos() = worldPos;
	this->SetDir(this->dir);
	this->dir = glm::normalize(dir);
	this->isOpen = false;
}

Exit::~Exit()
{
}

void Exit::SetDir(glm::vec3 newDir)
{
	float angle = glm::acos(newDir.x);
	this->dir = newDir;

	// Check if under the unit circle
	if (glm::sin(newDir.z) < 0.0)
		angle *= -1;

	this->transform.GetRot().y = angle;
}

glm::vec3 Exit::GetDir()
{
	return this->dir;
}

glm::mat4 Exit::GetWorldMatrix() const
{
	return this->transform.GetWorldMatrix();
}

Transform * Exit::GetTransform()
{
	return &this->transform;
}

glm::vec3 Exit::GetExitPos()
{
	return this->GetTransform()->GetPos();
}

glm::vec2 Exit::GetExitUVPos()
{
	return this->uvPos;
}

void Exit::Draw(Shader * shader)
{
	this->model->Draw(shader);
}
