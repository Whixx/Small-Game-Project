#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <iostream>
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <stdio.h>
#include "Mesh.h"
#include "Transform.h"

#include <glm\glm.hpp>
#include <string>

using namespace std;

const int MAX_NUMBER_OF_LIGHTS = 256;
const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const int NEAR_PLANE = 1, FAR_PLANE = 100;

class PointLight
{
public:
	PointLight();
	Mesh& GetMesh();

	glm::vec3& GetPos();
	glm::vec3& GetScale();
	glm::vec3& GetColor();
	
	Transform *GetTransform();
	vector<glm::mat4> &GetShadowTransforms();

	void CreateShadowTransforms();
	void ResetShadowTransforms();

	void Draw();
	virtual ~PointLight();

private:
	Mesh mesh;
	Transform transform;
	glm::mat4 shadowProj;
	vector<glm::mat4> shadowTransforms;
	glm::vec3 color;
};

class PointLightHandler
{
public:
	PointLightHandler();

	// location, pos, color
	void CreateLight(glm::vec3 position, glm::vec3 color);

	void SendToShader();
	void InitiateLights(GLuint *program);
	void UpdateShadowTransform(GLuint cameraIndex);
	void Draw(int index);
	
	vector<glm::mat4> GetShadowTransform(int index);

	GLuint GetNrOfLights();
	Transform *GetTransform(int index);

	virtual ~PointLightHandler();

private:
	PointLight lightArray[MAX_NUMBER_OF_LIGHTS];
	GLuint nrOfLights;

	GLuint loc_position[MAX_NUMBER_OF_LIGHTS];
	GLuint loc_color[MAX_NUMBER_OF_LIGHTS];
	GLuint loc_NrOfLights;
};

#endif