#include "ShadowMap.h"


ShadowMap::ShadowMap(unsigned int width, unsigned int height)
{
	this->fbo = 0;
	this->depthCubeMap = 0;

	this->width = width;
	this->height = height;

	this->Init();
}


ShadowMap::~ShadowMap()
{
	if (this->fbo != 0)
	{
		glDeleteFramebuffers(1, &this->fbo);
	}


	if (this->depthCubeMap != 0)
	{
		glDeleteTextures(1, &this->depthCubeMap);
	}
}

bool ShadowMap::Init()
{
	bool success = true;

	glGenFramebuffers(1, &this->fbo);

	// create depth cubemap texture
	glGenTextures(1, &this->depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubeMap);

	// Create 6 textures (a cubemap)
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Invisible borders
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMap, 0);

	// Specify that we aren't using any colorattachments, whichs means we're not gonna save anything other then the depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", Status);
		success = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return success;
}

void ShadowMap::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void ShadowMap::BindForReading(GLuint textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubeMap);
}
