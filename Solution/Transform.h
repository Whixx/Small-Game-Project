#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3& trans = glm::vec3(), const glm::vec3& rot = glm::vec3(),  const glm::vec3& scale = glm::vec3(1.0f,1.0f,1.0f)):
		m_trans(trans),
		m_rot(rot),
		m_scale(scale) {}

	inline glm::mat4 getWorldMatrix() const
	{
		glm::mat4 translationMatrix = glm::translate(m_trans);

		glm::mat4 rotationXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotationYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotationZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
		glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

		glm::mat4 scaleMatrix = glm::scale(m_scale);

		glm::mat4 worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		return worldMatrix;
	}

	inline glm::vec3& GetPos() { return m_trans; }
	inline glm::vec3& GetRot() { return m_rot; }
	inline glm::vec3& GetScale() { return m_scale; }

	inline void SetPos(const glm::vec3& trans) { m_trans = trans; }
	inline void SetRot(const glm::vec3& rot) { m_rot = rot; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }
private:
	glm::vec3 m_trans;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif
