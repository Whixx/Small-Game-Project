#ifndef EXIT_H
#define EXIT_H

#include "glm/glm.hpp"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

struct ExitPosDir
{
	glm::vec2 uvPos;
	glm::vec2 uvDir;
};

class Exit
{
public:
	Exit(Model* exitModelOpen, Model* exitModelClosed, glm::vec3 worldPos = glm::vec3(-1.0), glm::vec3 dir = glm::vec3(-1.0), glm::vec2 uvPos = glm::vec2(-1.0));
	Exit() {}
	~Exit();

	glm::mat4 GetWorldMatrix() const;
	Transform * GetTransform();
	glm::vec3 GetExitPos();
	glm::vec2 GetExitUVPos();
	glm::vec3 GetDir();

	void SetDir(glm::vec3 newDir);

	void DrawOpen(Shader* shader);
	void DrawClosed(Shader * shader);
private:
	bool isOpen;
	Transform transform;
	Model *modelOpen;
	Model *modelClosed;

	glm::vec2 uvPos;
	glm::vec3 dir;
};

#endif