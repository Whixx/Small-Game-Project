#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3& trans = glm::vec3(), const glm::vec3& rot = glm::vec3(),  const glm::vec3& scale = glm::vec3(1.0f,1.0f,1.0f)):
		trans(trans),
		rot(rot),
		scale(scale) {}

	inline glm::mat4 GetWorldMatrix() const
	{
		glm::mat4 translationMatrix = glm::translate(this->trans);

		glm::mat4 rotationXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotationYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotationZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
		glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

		glm::mat4 scaleMatrix = glm::scale(this->scale);

		glm::mat4 worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		return worldMatrix;
	}

	inline glm::vec3& GetPos() { return this->trans; }
	inline glm::vec3& GetRot() { return this->rot; }
	inline glm::vec3& GetScale() { return this->scale; }

	inline void SetPos(const glm::vec3& trans) { this->trans = trans; }
	inline void SetRot(const glm::vec3& rot) { this->rot = rot; }
	inline void SetScale(const glm::vec3& scale) { this->scale = scale; }
private:
	glm::vec3 trans;
	glm::vec3 rot;
	glm::vec3 scale;
};

#endif
