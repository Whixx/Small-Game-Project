#include "Blur.h"

BlurBuffer::BlurBuffer()
{
	m_fbo[0] = 0;
	m_fbo[1] = 0;
}

BlurBuffer::~BlurBuffer()
{
	if (m_fbo[0] != 0)
	{
		glDeleteFramebuffers(2, m_fbo);
	}

	if (m_colorBuffers[0] != 0)
	{
		glDeleteTextures(2, m_colorBuffers);
	}
}

bool BlurBuffer::Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	bool finish = true;

	glGenFramebuffers(2, m_fbo);

	glGenTextures(2, m_colorBuffers);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[i]);
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);

		// Allocate Storage for the gBuffer Textures
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach the texture to the framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffers[i], 0);
	}

	// Felcheckar
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		finish = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return finish;
}

void BlurBuffer::bindForWriting(bool horizontal)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo[horizontal]);
}

void BlurBuffer::bindForReading(bool horizontal, int textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo[horizontal]);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, m_colorBuffers[horizontal]);
}

void BlurBuffer::setReadBuffer(BLURBUFFER_TEXTURE_TYPE TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

