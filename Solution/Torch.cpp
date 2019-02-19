#include "Torch.h"

Torch::Torch(Transform transform, Mesh * mesh, Texture * texture, PointLightHandler* PLH, glm::vec3 lightColor)
{
	this->texture = texture;
	this->mesh = mesh;
	this->mesh->CreateMesh("ObjectFiles/torch.obj");
	this->transform = transform;
	this->transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	this->transform.GetRot() = glm::vec3(0.0f, 1.0f, 1.0f);

	glm::vec4 lightPos(0, 1, 0, 0);

	glm::mat4 scaleMatrix = glm::scale(glm::vec3(0, this->transform.GetScale().y, 0));

	glm::mat4 rotationXMatrix = glm::rotate(this->transform.GetRot().x, glm::vec3(1, 0, 0));
	glm::mat4 rotationYMatrix = glm::rotate(this->transform.GetRot().y, glm::vec3(0, 1, 0));
	glm::mat4 rotationZMatrix = glm::rotate(this->transform.GetRot().z, glm::vec3(0, 0, 1));
	glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

	glm::mat4 scaleRotMatrix = rotationMatrix * scaleMatrix;

	lightPos = scaleRotMatrix * lightPos;
	
	this->torchLight = PLH->CreateLight(lightPos, lightColor, 1.0f);
}

Torch::Torch()
{
}

Torch::~Torch()
{
}

void Torch::SetScale(glm::vec3 scale)
{
	this->transform.SetScale(scale);
}

void Torch::SetPos(glm::vec3 pos)
{
	this->transform.SetPos(pos);
}

void Torch::SetRot(glm::vec3 rot)
{
	this->transform.SetRot(rot);
}

void Torch::SetTransform(Transform transform)
{
	this->transform = transform;
}

glm::vec3 & Torch::GetScale()
{
	return this->transform.GetScale();
}

glm::vec3 & Torch::GetPos()
{
	return this->transform.GetPos();
}

glm::vec3 & Torch::GetRot()
{
	return this->transform.GetRot();
}

Transform Torch::GetTransform()
{
	return this->transform;
}

void Torch::BindTexture()
{
	this->texture->Bind(0);
}

void Torch::Draw()
{
	this->mesh->Draw();
}

void Torch::Update(double dt, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camRight, glm::vec3 camUp, float distFromPlayer)
{
	// Update the torch so that it is located in front of the player

	this->GetPos() = camPos
		+ camForward * distFromPlayer
		+ camRight * 0.4f
		+ camUp * -0.5f;

	//printf("%f, %f, %f \n", camForward.x, camForward.y, camForward.z);

	//glm::vec3 forward = glm::vec3(this->transform.GetWorldMatrix()[2][0], this->transform.GetWorldMatrix()[2][1], this->transform.GetWorldMatrix()[2][2]);
	//glm::vec3 camToTorch = glm::vec3(this->transform.GetPos() - camPos);
	//glm::vec3 crossVect = normalize(glm::cross(camToTorch, forward));
	//
	//if (crossVect.y > 0.3f)
	//{
	//	this->transform.GetRot().y -= dt * 10;
	//}
	//else if (crossVect.y < -0.3f)
	//{
	//	this->transform.GetRot().y += dt * 10;
	//}
	//else
	//{
	//	// No rotation
	//}


	// Update the lights position (Should be in the correct spot on the torch)
	glm::vec4 lightPos = glm::vec4(0, 1, 0, 0);

	glm::mat4 scaleMatrix = glm::scale(glm::vec3(0, this->transform.GetScale().y, 0));

	glm::mat4 rotationXMatrix = glm::rotate(this->transform.GetRot().x, glm::vec3(1, 0, 0));
	glm::mat4 rotationYMatrix = glm::rotate(this->transform.GetRot().y, glm::vec3(0, 1, 0));
	glm::mat4 rotationZMatrix = glm::rotate(this->transform.GetRot().z, glm::vec3(0, 0, 1));
	glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

	glm::mat4 scaleRotMatrix = rotationMatrix * scaleMatrix;

	lightPos = scaleRotMatrix * lightPos;
	
	this->torchLight->GetPos() = glm::vec3(lightPos.x, lightPos.y, lightPos.z);
}