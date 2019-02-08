#include "MainFunctions.h"


void updateAllObjects(double dt, ObjectHandler & OH)
{
	for (int i = 0; i < OH.getNrOfObjects(); i++)
	{
		OH.getObject(i)->Update(dt);
	}
}

void shadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Camera *camera)
{
	glEnable(GL_DEPTH_TEST);
	shadowShader->Bind();
	shadowFBO->bind();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	vector<glm::mat4> shadowTransforms;
	glm::vec3 lightPos;

	// For each light, we create a matrix and draws each light.
	for (unsigned int i = 0; i < PLH->getNrOfLights(); i++)
	{
		shadowTransforms = PLH->getShadowTransform(i);

		lightPos = PLH->getTransform(i)->GetPos();

		for (int j = 0; j < 6; ++j)
		{
			shadowShader->setMat4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);
		}
		shadowShader->sendFloat("farPlane", (float)FAR_PLANE);
		shadowShader->sendVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

		for (unsigned int j = 0; j < OH->getNrOfObjects(); j++)
		{
			shadowShader->Update(OH->getObject(j)->GetTransform(), *camera);
			OH->getObject(j)->bindTexture();
			OH->getObject(j)->Draw();
		}
	}

	shadowShader->unBind();
	glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);
	glDisable(GL_DEPTH_TEST);
}

void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, GLuint cameraLocationGP, GLint texLoc, GLint normalTexLoc, int torch)
{
	geometryPass->Bind();

	sendCameraLocationToGPU(cameraLocationGP, camera);
	prepareTexture(texLoc, normalTexLoc);

	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// Update and Draw all objects
	for (unsigned int i = 0; i < OH->getNrOfObjects(); i++)
	{
		if (OH->getObject(i) == OH->getObject(torch))
		{
			geometryPass->sendFloat("illuminated", 3.0f);
		}
		else
		{
			geometryPass->sendFloat("illuminated", 1.0f);
		}
		geometryPass->Update(OH->getObject(i)->GetTransform(), *camera);
		OH->getObject(i)->bindTexture();
		OH->getObject(i)->Draw();
	}

	geometryPass->unBind();
}

void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenTriangle, GLuint *program, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, GLuint cameraLocationLP, Camera *camera)
{
	lightPass->Bind();

	lights->sendToShader();
	sendCameraLocationToGPU(cameraLocationLP, camera);

	// Bloom buffer, write finalColor and brightness.
	bloomBuffer->bindForWriting();
	bloomBuffer->bindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	lightPass->sendInt("gPosition", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	lightPass->sendInt("gDiffuse", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	lightPass->sendInt("gNormal", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	shadowBuffer->bindForReading(3);
	lightPass->sendInt("shadowMap", 3);
	lightPass->sendFloat("farPlane", (float)FAR_PLANE);

	lightPass->validateShaders();

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

	lightPass->unBind();
}

// This function draws particles to the screen.
void particlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position)
{
	finalFBO->bindForWriting();

	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	// PS = ParticleShader
	GLuint cameraRightWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraRightWorldPS");
	GLuint cameraUpWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraUpWorldPS");
	GLuint viewProjection = glGetUniformLocation(*particleShader->getProgram(), "viewProjectionMatrix");

	// Create new particles
	particle->generateParticles(deltaTime, position);

	// Simulate all the particles
	particle->simulateParticles(camera->getCameraPosition(), deltaTime);

	// Update the buffers holding the particles
	particle->update();

	// Bind the correct shader
	particleShader->Bind();

	// Send Uniforms
	glUniform3f(cameraRightWorldPS, camera->getRightVector().x, camera->getRightVector().y, camera->getRightVector().z);
	glUniform3f(cameraUpWorldPS, camera->getUpVector().x, camera->getUpVector().y, camera->getUpVector().z);
	glUniformMatrix4fv(viewProjection, 1, GL_FALSE, &camera->getViewProjection()[0][0]);

	// Disable depthbuffer and enable blend
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	particle->bindTexture();

	// Bind the buffers holding the particles
	particle->bind();

	// Draw the particles, send a draw call to the GPU
	particle->draw();

	glDisable(GL_BLEND);
	glDepthMask(true);

	// Unbind the shader
	particleShader->unBind();
}

void lightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, double counter)
{
	bloomBuffer->bindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	pointLightPass->Bind();
	for (unsigned int i = 0; i < lights->getNrOfLights(); i++)
	{
		pointLightPass->Update(*lights->getTransform(i), *camera);
		lights->Draw(i);
	}
	pointLightPass->unBind();
	glDisable(GL_CULL_FACE);
}

void blurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	blurShader->Bind();
	int timesToBlur = 10;
	bool horizontal = true;
	bool firstBlur = true;

	//GLuint loc = glGetUniformLocation(*blurShader->getProgram(), "scene");

	for (int i = 0; i < timesToBlur; i++)
	{
		blurBuffers->bindForWriting(!horizontal);

		// First we read from the bloom fbo, to get a starting point of the blur. Then after that we will
		// blur the "blurred" texture over and over again, swapping between vertical and horizontal blurring.
		if (firstBlur == true)
		{
			bloomBuffer->bindForReadingBloomMap(0);
			blurShader->sendInt("horizontal", !horizontal);
			blurShader->sendInt("scene", 0);
		}
		else
		{
			blurBuffers->bindForReading(horizontal, 0);
			blurShader->sendInt("horizontal", !horizontal);

			blurShader->sendInt("scene", 0);
		}

		glDisable(GL_DEPTH_TEST);
		fullScreenTriangle->Draw();
		glEnable(GL_DEPTH_TEST);

		horizontal = !horizontal;
		if (firstBlur)
		{
			firstBlur = false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void finalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	finalFBO->bindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	finalBloomShader->Bind();

	bloomBuffer->bindForReadingDiffuse();
	finalBloomShader->sendInt("scene", 0);

	blurBuffers->bindForReading(1, 1);
	finalBloomShader->sendInt("bright", 1);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

}

void finalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh * fullScreenTriangle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	finalFBO->bindForReading(0);

	finalShader->Bind();
	finalShader->sendInt("scene", 0);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);
}

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera)
{
	glUniform3f(cameraLocation, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
}

void prepareTexture(GLuint textureLoc, GLuint normalMapLoc)
{
	glUniform1i(textureLoc, 0);
	glUniform1i(normalMapLoc, 1);
}

void setStartPositions(ObjectHandler * OH)
{

}

void generateMazeBitmaps(int height, int width)
{
	MazeGeneratePNG mazeGen(height, width);

	// set_cell can be used to set "entrance and exit" etc
	//mazeGen.set_cell(0, 1, mazeGen.path);
	//mazeGen.set_cell(mazeHeight - 1, mazeWidth - 2, mazeGen.path);

	mazeGen.generate();
	mazeGen.draw_png();
}