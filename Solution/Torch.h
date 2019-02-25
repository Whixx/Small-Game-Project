#ifndef TORCH_H
#define TORCH_H

#include <glew\glew.h>
#include "Transform.h"
#include "PointLight.h"
#include "Particle.h"
#include "SoundHandler.h"
#include "Model.h"

class Torch
{
public:
	Torch(Transform transform, glm::vec3 lightColor, irrklang::ISoundEngine* engine, PointLightHandler* PLH, float torchSize);
	~Torch();

	void SetScale(glm::vec3 scale);
	void SetPos(glm::vec3 pos);
	void SetRot(glm::vec3 rot);
	void SetTransform(Transform transform);

	glm::vec3 &GetScale();
	glm::vec3 &GetPos();
	glm::vec3 &GetRot();
	Transform GetTransform();
	Model* GetModel();
	Particle* GetParticle();

	glm::vec3 GetFirePos();

	void BindMaterial(Shader* shader);
	void Draw(Shader* shader);
	virtual void Update(double dt, Camera camera, glm::vec3 camForward, float distFromPlayer);
	
private:
	Model model;
	Particle particle;
	Transform transform;
	SoundHandler torchSound;
	float size;
	PointLight* torchLight;
	const glm::vec4 lightStartingPos = glm::vec4(0, 4.6f, 0, 0);
	glm::vec3 lightPos;
};

#endif
