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
	void BindForWriting();
	void BindForReading();

	void BindForReadingBloomMap(int textureUnit);
	void BindForReadingDiffuse();

	void CopyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead);

	GLuint GetFBO();

private:
	GLuint fbo;
	GLuint colorBuffers[BLOOMBUFFER_NUM_TEXTURES];
	GLuint depthTexture;
};

#endif