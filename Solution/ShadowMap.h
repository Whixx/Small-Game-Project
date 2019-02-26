#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <glew\glew.h>
#include <iostream>

class ShadowMap
{
public:
	ShadowMap(unsigned int mapWidth, unsigned int mapHeight);
	~ShadowMap();

	bool Init();

	// Binds the texture as a target during the geometry pass
	void Bind();

	void BindForReading(GLuint textureUnit);
private:
	GLuint fbo;
	GLuint depthCubeMap;

	unsigned int width;
	unsigned int height;
};
#endif
