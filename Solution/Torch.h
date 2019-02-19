#ifndef TORCH_H
#define TORCH_H

#include <glew\glew.h>
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "SoundHandler.h"

class Torch
{
public:
	Torch(Transform transform, Mesh * mesh, Texture * texture, irrklang::ISoundEngine* engine);
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
	virtual void Update(double dt);
	
private:
	Mesh *mesh;
	Texture *texture;
	Transform transform;
	SoundHandler torchSound;
};

#endif
