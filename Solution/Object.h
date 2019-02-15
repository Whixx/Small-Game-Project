#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"

using namespace std;

class Object
{
public:
	Object(Mesh *mesh, Texture *texture);
	// Default constructor
	Object();
	virtual ~Object();

	glm::mat4 GetWorldMatrix() const;
	Transform GetTransform() const;
	glm::vec3& GetPos();
	glm::vec3& GetRot();
	glm::vec3& GetScale();

	virtual void Update(double dt);
	virtual void Draw();

	void BindTexture();

private:
	Mesh *mesh;
	Transform transform;
	Texture *texture;

	unsigned int id;
};

#endif //DISPLAY_H