#include "Torch.h"

Torch::Torch(Transform transform, Mesh * mesh, Texture * texture, irrklang::ISoundEngine* engine)
	:torchSound("Sounds/torch.wav", true, false, engine)
{
	this->texture = texture;
	this->mesh = mesh;
	this->mesh->CreateMesh("ObjectFiles/torch.obj");
	this->transform = transform;
	this->transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
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
	transform.SetPos(this->GetPos());

	this->torchSound.SetPosition(this->GetPos());
	torchSound.Play();
}

