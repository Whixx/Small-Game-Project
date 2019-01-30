#pragma once

#include <glew\glew.h>
#include <iostream>

class BlurBuffer
{
public:
	enum BLURBUFFER_TEXTURE_TYPE
	{
		BLURBUFFER_TEXTURE_TYPE_BLUR,
		BLURBUFFER_NUM_TEXTURES
	};

	BlurBuffer();
	~BlurBuffer();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	void bindForWriting(bool horizontal);
	void bindForReading(bool horizontal, int textureUnit);
	void setReadBuffer(BLURBUFFER_TEXTURE_TYPE TextureType);
private:
	GLuint m_fbo[2];
	GLuint m_colorBuffers[2];
};

