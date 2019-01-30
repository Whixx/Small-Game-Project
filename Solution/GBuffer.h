#ifndef GBUFFER_H
#define GBUFFER_H

#include <glew\glew.h>
#include <iostream>

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
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);

	// Binds the texture as a target during the geometry pass
	void BindForWriting();
	// Binds the FBO(Frame Buffer Object) as input so its contents can be dumped to the screen
	// A FBO is a container for textures and an optional depth buffer
	void BindForReading();

	GLuint getFBO();
private:
	
	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};
#endif