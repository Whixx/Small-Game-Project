#ifndef BLOOM_H
#define BLOOM_H

#include <glew\glew.h>
#include <iostream>

class BloomBuffer
{
public:
	enum BLOOMBUFFER_TEXTURE_TYPE
	{	
		BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE,
		BLOOMBUFFER_TEXTURE_TYPE_BLOOMMAP,
		BLOOMBUFFER_NUM_TEXTURES
	};

	BloomBuffer();
	~BloomBuffer();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	void bindForWriting();
	void bindForReading();

	void bindForReadingBloomMap(int textureUnit);
	void bindForReadingDiffuse();

	void copyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead);

	GLuint getFBO();

private:
	GLuint m_fbo;
	GLuint m_colorBuffers[BLOOMBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};

#endif