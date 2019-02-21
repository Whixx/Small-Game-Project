#ifndef TORCH_H
#define TORCH_H

#include <glew\glew.h>
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "PointLight.h"
#include "Particle.h"
#include "SoundHandler.h"

class Torch
{
public:
	Torch(Transform transform, Mesh * mesh, Texture * texture, glm::vec3 lightColor, irrklang::ISoundEngine* engine, PointLightHandler* PLH);
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

	Particle &GetParticle();
	glm::vec3 GetFirePos();

	void BindTexture();
	void Draw();
	virtual void Update(double dt, Transform transform, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camRight, glm::vec3 camUp, float distFromPlayer);
	
private:
	Mesh *mesh;
	Texture *texture;
	Transform transform;
	SoundHandler torchSound;
	const float size = 0.02f;
	PointLight* torchLight;
	Particle particle;
	Texture particleTexture;
	const glm::vec4 lightStartingPos = glm::vec4(0, 4.6f, 0, 0);
	glm::vec3 lightPos;
};

#endif
