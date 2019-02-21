#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include <glew\glew.h>
#include "Camera.h"
#include <unordered_map>

class Shader
{
public:
	Shader();

	void Bind();
	void UnBind();
	void InitiateShaders(bool color = false);
	void ValidateShaders();

	GLuint CreateShader(const std::string& fileName, GLenum shaderType);
	GLuint *GetProgram();

	void SendInt(const char *name, int value);
	void SendFloat(const char *name, float value);
	void SendVec3(const char *name, float x, float y, float z);
	void SendMat4(const char *name, const glm::mat4 &mat);
	void SendCameraLocation(Camera *camera);

	virtual ~Shader();
private:
	unsigned int NUM_OF_SHADERS;

	// holds all the uniform locations
	std::unordered_map<std::string, int> m_UniformLocationCache;

	GLuint program;
	GLuint shaders[32];

	// Fetches location from map, if not in it, get location from program
	int GetUniformLocation(const std::string& name);
};

#endif //SHADER_H