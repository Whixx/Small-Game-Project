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
	void BindForWriting(bool horizontal);
	void BindForReading(bool horizontal, int textureUnit);
	void SetReadBuffer(BLURBUFFER_TEXTURE_TYPE TextureType);

private:
	GLuint fbo[2];
	GLuint colorBuffers[2];
};

