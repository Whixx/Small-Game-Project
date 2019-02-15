#include "Torch.h"

Torch::Torch(Transform transform, Mesh * mesh, Texture * texture, PointLightHandler* PLH, glm::vec3 lightColor)
{
	this->texture = texture;
	this->mesh = mesh;
	this->mesh->CreateMesh("ObjectFiles/torch.obj");
	this->transform = transform;
	this->transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	glm::vec3 lightPos = glm::vec3(transform.GetPos().x, transform.GetPos().y + 1.0f * transform.GetScale().y, transform.GetPos().z);
	lightPos *= transform.GetRot().x*transform.GetRot().y*transform.GetRot().z;
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

void Torch::Update(double dt)
{
	//transform.SetPos(this->GetPos());
}

