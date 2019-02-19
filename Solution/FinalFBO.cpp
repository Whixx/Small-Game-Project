#include "FinalFBO.h"



FinalFBO::FinalFBO(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	this->fbo = 0;
	this->depthTexture = 0;

	this->width = SCREENWIDTH;
	this->height = SCREENHEIGHT;

	this->Init();
}


FinalFBO::~FinalFBO()
{
	if (this->fbo != 0)
	{
		glDeleteFramebuffers(1, &this->fbo);
	}

	if (this->colorBuffers[0] != 0)
	{
		glDeleteTextures(FINALFBO_NUM_TEXTURES, this->colorBuffers);
	}

	if (this->depthTexture != 0)
	{
		glDeleteTextures(1, &this->depthTexture);
	}
}

bool FinalFBO::Init()
{
	bool finish = true;

	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	glGenTextures(FINALFBO_NUM_TEXTURES, this->colorBuffers);
	for (unsigned int i = 0; i < FINALFBO_NUM_TEXTURES; i++)
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
	unsigned int attachments[FINALFBO_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(FINALFBO_NUM_TEXTURES, attachments);

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

void FinalFBO::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
}

void FinalFBO::BindForReading(GLuint textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);

	// if we have different textures to bind, we need to change the current texture openGL is working with.
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
	glBindTexture(GL_TEXTURE_2D, this->colorBuffers[FINALFBO_TEXTURE_TYPE_DIFFUSE]);
}

void FinalFBO::CopyDepth(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT, GLuint fboRead)
{
	GLint drawFboId = 0, readFboId;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);

	if(readFboId != fboRead)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboRead);

	if(drawFboId != this->fbo)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

	glBlitFramebuffer(0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0, SCREENWIDTH, SCREENHEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

GLuint FinalFBO::GetFBO()
{
	return this->fbo;
}
