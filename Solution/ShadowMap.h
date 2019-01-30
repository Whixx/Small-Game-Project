#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <glew\glew.h>
#include <iostream>
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool Init();

	// Binds the texture as a target during the geometry pass
	void bind();

	void bindForReading(GLuint textureUnit);
private:
	GLuint m_fbo;
	GLuint m_depthCubeMap;
};
#endif
