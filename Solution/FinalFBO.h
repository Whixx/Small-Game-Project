#ifndef FINALFBO_H
#define FINALFBO_H

#include <glew\glew.h>
#include <iostream>

class FinalFBO
{
public:
	enum FINALBUFFER_TEXTURE_TYPE
	{
		FINALFBO_TEXTURE_TYPE_DIFFUSE,
		FINALFBO_NUM_TEXTURES
	};

	FinalFBO();
	~FinalFBO();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	void bindForWriting();
	void bindForReading(GLuint textureUnit);

	void copyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead);

	GLuint getFBO();
private:
	GLuint m_fbo;
	GLuint m_colorBuffers[FINALFBO_NUM_TEXTURES];
	GLuint m_depthTexture;
};

#endif