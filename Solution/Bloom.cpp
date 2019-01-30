#include "Bloom.h"

BloomBuffer::BloomBuffer()
{
	m_fbo = 0;
	m_depthTexture = 0;
}

BloomBuffer::~BloomBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_colorBuffers[0] != 0)
	{
		glDeleteTextures(BLOOMBUFFER_NUM_TEXTURES, m_colorBuffers);
	}

	if (m_depthTexture != 0)
	{
		glDeleteTextures(1, &m_depthTexture);
	}
}

bool BloomBuffer::Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	bool finish = true;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(BLOOMBUFFER_NUM_TEXTURES, m_colorBuffers);
	for (unsigned int i = 0; i < BLOOMBUFFER_NUM_TEXTURES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0);
	}


	// Tells openGL that we want two rendertargets
	unsigned int attachments[BLOOMBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(BLOOMBUFFER_NUM_TEXTURES, attachments);

	// Depth texture
	//glGenRenderbuffers(1, &m_depthTexture);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREENWIDTH, SCREENHEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);



	glGenTextures(1, &m_depthTexture);
	// Make the depthTexture active
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	// Allocate Storage for the depthTexture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCREENWIDTH, SCREENHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// Attach the depth texture to the framebuffer (GL_DEPTH_ATTATCHMENT)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	// Felcheckar
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		finish = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return finish;
}

void BloomBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void BloomBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	for (unsigned int i = 0; i < BLOOMBUFFER_NUM_TEXTURES; i++)
	{
		// if we have different textures to bind, we need to change the current texture openGL is working with.
		glActiveTexture(GL_TEXTURE0 + i);
		// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE + i]);
	}
}

void BloomBuffer::bindForReadingBloomMap(int textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, m_colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_BLOOMMAP]);
}

void BloomBuffer::bindForReadingDiffuse()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, m_colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE]);
}

void BloomBuffer::copyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboRead);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_fbo);
	glBlitFramebuffer(0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0, SCREENWIDTH, SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

GLuint BloomBuffer::getFBO()
{
	return this->m_fbo;
}
