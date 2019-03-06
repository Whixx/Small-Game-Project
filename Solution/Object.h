#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include <glm/gtx/norm.hpp>

using namespace std;

class Object
{
public:
	Object(string path = "Models/Cube/cube.obj");
	virtual ~Object();

	void SetDir(glm::vec3 newDir);

	glm::mat4 GetWorldMatrix() const;
	Transform GetTransform() const;
	glm::vec3& GetPos();
	glm::vec3& GetRot();
	glm::vec3& GetScale();

	virtual void Update(double dt);
	virtual void Draw(Shader* shader);

	Model* GetModel() const;
private:
	Model* model;
	Transform transform;
};

#endif //DISPLAY_H