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
	Object(Mesh *mesh, Texture *texture, unsigned int id);
	// Default constructor
	Object();
	virtual ~Object();

	glm::mat4 getWorldMatrix() const;
	Transform GetTransform() const;
	glm::vec3& GetPos();
	glm::vec3& GetRot();
	glm::vec3& GetScale();

	virtual void Update();
	virtual void Draw();

	void bindTexture();

private:
	Mesh *mesh;
	Transform transform;
	Texture *texture;

	unsigned int m_id;

};

#endif //DISPLAY_H