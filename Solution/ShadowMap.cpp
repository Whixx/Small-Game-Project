#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
	this->m_fbo = 0;
	this->m_depthCubeMap = 0;
}


ShadowMap::~ShadowMap()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}


	if (m_depthCubeMap != 0)
	{
		glDeleteTextures(1, &m_depthCubeMap);
	}
}

bool ShadowMap::Init()
{
	bool success = true;

	glGenFramebuffers(1, &m_fbo);

	// create depth cubemap texture
	glGenTextures(1, &m_depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubeMap);

	// Create 6 textures (a cubemap)
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
		
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubeMap, 0);

	// Specify that we aren't using any colorattachments, whichs means we're not gonna save anything other then the depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		success = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return success;
}

void ShadowMap::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);
}

void ShadowMap::bindForReading(GLuint textureUnit)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubeMap);
}
