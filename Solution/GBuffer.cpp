#include "GBuffer.h"

GBuffer::GBuffer()
{
	m_fbo = 0;
	m_depthTexture = 0;
}

GBuffer::~GBuffer()
{
	if (m_fbo != 0) 
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_textures[0] != 0) 
	{
		glDeleteTextures(GBUFFER_NUM_TEXTURES, m_textures);
	}

	if (m_depthTexture != 0) 
	{
		glDeleteTextures(1, &m_depthTexture);
	}
}

bool GBuffer::Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	bool finish = true;

	// Create the FBO (Frame Buffer Object)
	glGenFramebuffers(1, &m_fbo);
	// Bind the frambuffer (Make it active)
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Initialize vertex attributes textures (Allocate memory for the gBuffer textures)
	glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++)
	{
		// Make all the textures active
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		// Allocate Storage for the gBuffer Textures
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);

		// Cutting or adding the texture if the texture is larger than the object.
		// Example cutting: texture is 512:512, quad is 500:500, then these parameters does it so that the texture should be cut to 500:500
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Attach the texture to the framebuffer. All textures will be in GL_COLOR_ATTACHMENT0-1-2-3
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	// Tell OpenGl which color attachments we'll use (of this FBO) for rendering
	// Enables writing to all our textures
	GLenum attatchments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(GBUFFER_NUM_TEXTURES, attatchments);

	// Create the depthTexture
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

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return finish;
}

void GBuffer::BindForWriting()
{
	// Anledningen till att vi ska ha denna istället är för att vi specificerar vad vi ska göra med FBO:n.
	// Eftersom att vi ska skriva till den så är de onödigt att enabla både skriv/läs.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++)
	{	
		// if we have different textures to bind, we need to change the current texture openGL is working with.
		glActiveTexture(GL_TEXTURE0 + i);
		// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}
}

GLuint GBuffer::getFBO()
{
	return this->m_fbo;
}
