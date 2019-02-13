#include "Blur.h"

BlurBuffer::BlurBuffer(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	this->fbo[0] = 0;
	this->fbo[1] = 0;

	this->width = SCREENWIDTH;
	this->height = SCREENHEIGHT;

	this->Init();
}

BlurBuffer::~BlurBuffer()
{
	if (this->fbo[0] != 0)
	{
		glDeleteFramebuffers(2, this->fbo);
	}

	if (this->colorBuffers[0] != 0)
	{
		glDeleteTextures(2, this->colorBuffers);
	}
}

bool BlurBuffer::Init()
{
	bool finish = true;

	glGenFramebuffers(2, this->fbo);

	glGenTextures(2, this->colorBuffers);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo[i]);
		glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);

		// Allocate Storage for the gBuffer Textures
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width, this->height, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach the texture to the framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBuffers[i], 0);
	}

	// Felcheckar
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", Status);
		finish = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return finish;
}

void BlurBuffer::BindForWriting(bool horizontal)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo[horizontal]);
}

void BlurBuffer::BindForReading(bool horizontal, int textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo[horizontal]);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, this->colorBuffers[horizontal]);
}

void BlurBuffer::SetReadBuffer(BLURBUFFER_TEXTURE_TYPE TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

