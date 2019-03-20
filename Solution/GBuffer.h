#ifndef GBUFFER_H
#define GBUFFER_H

#include <glew\glew.h>
#include <iostream>
#include <vector>

/*
Detta sker i två steg: Geometry pass och lightning pass.
*/
class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_SPECULAR_SHININESS_HEIGHT,
		GBUFFER_TEXTURE_TYPE_EMISSIVE,
		GBUFFER_TEXTURE_TYPE_AMBIENT,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	~GBuffer();

	bool Init();

	// Binds the texture as a target during the geometry pass
	void BindForWriting();
	// Binds the FBO(Frame Buffer Object) as input so its contents can be dumped to the screen
	// A FBO is a container for textures and an optional depth buffer
	void BindForReading();

	GLuint GetFBO();

private:
	GLuint fbo;
	GLuint textures[GBUFFER_NUM_TEXTURES];
	GLuint depthTexture;

	unsigned int width;
	unsigned int height;
};
#endif