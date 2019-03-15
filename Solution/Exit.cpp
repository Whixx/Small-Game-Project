#include "Exit.h"

Exit::Exit(Model* exitModelOpen, Model* exitModelClosed, glm::vec3 worldPos, glm::vec3 dir, glm::vec2 uvPos)
{
	this->modelOpen = exitModelOpen;
	this->modelClosed = exitModelClosed;
	this->uvPos = uvPos;
	this->transform.GetPos() = worldPos;
	this->SetDir(this->dir);
	this->dir = glm::normalize(dir);
	this->isOpen = false;

	/*exit.GetTransform()->SetScale(glm::vec3(
		0.11f * maze.GetTransform()->GetScale().x,
		0.08f * maze.GetTransform()->GetScale().y,
		0.11f * maze.GetTransform()->GetScale().z));*/

	

	// Choose a good position
	if (this->dir.x < 0.0f)
	{
		this->transform.GetPos() = worldPos + this->dir * 0.8f;
	}
	else if (this->dir.x > 0.0f)
	{
		this->transform.GetPos() = worldPos + this->dir * 0.8f;
	}
	else if (this->dir.z > 0.0f)
	{
		this->transform.GetPos() = worldPos - this->dir * 0.8f;
	}
	else if (this->dir.z < 0.0f)
	{
		this->transform.GetPos() = worldPos - this->dir * 0.8f;
	}
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
	{
		angle *= -1;
	}

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

void Exit::DrawOpen(Shader * shader)
{
	this->modelOpen->Draw(shader);
}

void Exit::DrawClosed(Shader * shader)
{
	this->modelClosed->Draw(shader);
}
