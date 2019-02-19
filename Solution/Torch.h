#ifndef TORCH_H
#define TORCH_H

#include <glew\glew.h>
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "PointLight.h"

class Torch
{
public:
	Torch(Transform transform, Mesh * mesh, Texture * texture, PointLightHandler* PLH, glm::vec3 lightColor);
	Torch();
	~Torch();

	void SetScale(glm::vec3 scale);
	void SetPos(glm::vec3 pos);
	void SetRot(glm::vec3 rot);
	void SetTransform(Transform transform);

	glm::vec3 &GetScale();
	glm::vec3 &GetPos();
	glm::vec3 &GetRot();
	Transform GetTransform();

	void BindTexture();
	void Draw();
	virtual void Update(double dt, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camRight, glm::vec3 camUp, float distFromPlayer);
	
private:
	Mesh *mesh;
	Texture *texture;
	Transform transform;
	PointLight* torchLight;
};

#endif
