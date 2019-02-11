#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include <glew\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader();

	void Bind();
	void UnBind();
	void Update(const Transform& transform, const Camera& camera);
	void InitiateShaders(bool color);
	void ValidateShaders();


	GLuint CreateShader(const std::string& fileName, GLenum shaderType);
	GLuint *GetProgram();

	void SendInt(const char *name, int value);
	void SendFloat(const char *name, float value);
	void SendVec3(const char *name, float x, float y, float z);
	void SetMat4(const std::string &name, const glm::mat4 &mat);
	void SendMat4(const char *name, const glm::mat4 &mat);

	virtual ~Shader();
private:
	unsigned int NUM_OF_SHADERS;

	enum
	{
		TRANSFORM_U,
		WORLD_U,

		NUM_OF_UNIFORMS
	};

	GLuint program;
	GLuint shaders[32];
	GLuint uniforms[32];
};

#endif //SHADER_H