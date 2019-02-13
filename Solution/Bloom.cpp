#include "Bloom.h"

BloomBuffer::BloomBuffer(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	this->fbo = 0;
	this->depthTexture = 0;

	this->width = SCREENWIDTH;
	this->height = SCREENHEIGHT;

	this->Init();
}

BloomBuffer::~BloomBuffer()
{
	if (this->fbo != 0)
	{
		glDeleteFramebuffers(1, &this->fbo);
	}

	if (this->colorBuffers[0] != 0)
	{
		glDeleteTextures(BLOOMBUFFER_NUM_TEXTURES, this->colorBuffers);
	}

	if (this->depthTexture != 0)
	{
		glDeleteTextures(1, &this->depthTexture);
	}
}

bool BloomBuffer::Init()
{
	bool finish = true;

	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	glGenTextures(BLOOMBUFFER_NUM_TEXTURES, this->colorBuffers);
	for (unsigned int i = 0; i < BLOOMBUFFER_NUM_TEXTURES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width, this->height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffers[i], 0);
	}

	// Tells openGL that we want two rendertargets
	unsigned int attachments[BLOOMBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(BLOOMBUFFER_NUM_TEXTURES, attachments);

	// Depth texture
	//glGenRenderbuffers(1, &depthTexture);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthTexture);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREENWIDTH, SCREENHEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);

	glGenTextures(1, &this->depthTexture);
	// Make the depthTexture active
	glBindTexture(GL_TEXTURE_2D, this->depthTexture);
	// Allocate Storage for the depthTexture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// Attach the depth texture to the framebuffer (GL_DEPTH_ATTATCHMENT)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);

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

void BloomBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
}

void BloomBuffer::BindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);

	for (unsigned int i = 0; i < BLOOMBUFFER_NUM_TEXTURES; i++)
	{
		// if we have different textures to bind, we need to change the current texture openGL is working with.
		glActiveTexture(GL_TEXTURE0 + i);
		// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
		glBindTexture(GL_TEXTURE_2D, this->colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE + i]);
	}
}

void BloomBuffer::BindForReadingBloomMap(int textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, this->colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_BLOOMMAP]);
}

void BloomBuffer::BindForReadingDiffuse()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, colorBuffers[BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE]);
}

void BloomBuffer::CopyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboRead);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
	glBlitFramebuffer(0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0, SCREENWIDTH, SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

GLuint BloomBuffer::GetFBO()
{
	return this->fbo;
}
