#ifndef TORCH_H
#define TORCH_H

#include <glew\glew.h>
#include "Transform.h"
#include "Model.h"

class Torch
{
public:
	Torch(const string& path = "Models/Torch/torch.obj");
	~Torch();

	void SetScale(glm::vec3 scale);
	void SetPos(glm::vec3 pos);
	void SetRot(glm::vec3 rot);
	void SetTransform(Transform transform);

	glm::vec3 &GetScale();
	glm::vec3 &GetPos();
	glm::vec3 &GetRot();
	Transform GetTransform();

	void Draw(Shader* shader);
	virtual void Update(double dt);
	
private:
	Model* model;
	Transform transform;
};

#endif
