#include "MainFunctions.h"


void updateAllObjects(double dt, ObjectHandler & OH)
{
	for (int i = 0; i < OH.GetNrOfObjects(); i++)
	{
		OH.GetObject(i)->Update(dt);
	}
}

void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Camera *camera)
{
	glEnable(GL_DEPTH_TEST);
	shadowShader->Bind();
	shadowFBO->Bind();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	vector<glm::mat4> shadowTransforms;
	glm::vec3 lightPos;

	// For each light, we create a matrix and draws each light.
	for (unsigned int i = 0; i < PLH->GetNrOfLights(); i++)
	{
		shadowTransforms = PLH->GetShadowTransform(i);

		lightPos = PLH->GetTransform(i)->GetPos();

		for (int j = 0; j < 6; ++j)
		{
			shadowShader->SetMat4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);
		}
		shadowShader->SendFloat("farPlane", (float)FAR_PLANE);
		shadowShader->SendVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

		for (unsigned int j = 0; j < OH->GetNrOfObjects(); j++)
		{
			shadowShader->Update(OH->GetObject(j)->GetTransform(), *camera);
			OH->GetObject(j)->BindTexture();
			OH->GetObject(j)->Draw();
		}
	}

	shadowShader->UnBind();
	glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);
	glDisable(GL_DEPTH_TEST);
}

void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, GLuint cameraLocationGP, GLint texLoc, GLint normalTexLoc, int torch)
{
	geometryPass->Bind();

	SendCameraLocationToGPU(cameraLocationGP, camera);
	PrepareTexture(texLoc, normalTexLoc);

	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// Update and Draw all objects
	for (unsigned int i = 0; i < OH->GetNrOfObjects(); i++)
	{
		if (OH->GetObject(i) == OH->GetObject(torch))
		{
			geometryPass->SendFloat("illuminated", 3.0f);
		}
		else
		{
			geometryPass->SendFloat("illuminated", 1.0f);
		}
		geometryPass->Update(OH->GetObject(i)->GetTransform(), *camera);
		OH->GetObject(i)->BindTexture();
		OH->GetObject(i)->Draw();
	}

	geometryPass->UnBind();
}

void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenTriangle, GLuint *program, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, GLuint cameraLocationLP, Camera *camera)
{
	lightPass->Bind();

	lights->SendToShader();
	SendCameraLocationToGPU(cameraLocationLP, camera);

	// Bloom buffer, write finalColor and brightness.
	bloomBuffer->BindForWriting();
	bloomBuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	lightPass->SendInt("gPosition", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	lightPass->SendInt("gDiffuse", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	lightPass->SendInt("gNormal", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	shadowBuffer->BindForReading(3);
	lightPass->SendInt("shadowMap", 3);
	lightPass->SendFloat("farPlane", (float)FAR_PLANE);

	lightPass->ValidateShaders();

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

	lightPass->UnBind();
}

// This function draws particles to the screen.
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position)
{
	finalFBO->BindForWriting();

	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	// PS = ParticleShader
	GLuint cameraRightWorldPS = glGetUniformLocation(*particleShader->GetProgram(), "cameraRightWorldPS");
	GLuint cameraUpWorldPS = glGetUniformLocation(*particleShader->GetProgram(), "cameraUpWorldPS");
	GLuint viewProjection = glGetUniformLocation(*particleShader->GetProgram(), "viewProjectionMatrix");

	// Create new particles
	particle->GenerateParticles(deltaTime, position);

	// Simulate all the particles
	particle->SimulateParticles(camera->GetCameraPosition(), deltaTime);

	// Update the buffers holding the particles
	particle->Update();

	// Bind the correct shader
	particleShader->Bind();

	// Send Uniforms
	glUniform3f(cameraRightWorldPS, camera->GetViewProjection()[0][0], camera->GetViewProjection()[1][0], camera->GetViewProjection()[2][0]);
	glUniform3f(cameraUpWorldPS, camera->GetViewProjection()[0][1], camera->GetViewProjection()[1][1], camera->GetViewProjection()[2][1]);
	glUniformMatrix4fv(viewProjection, 1, GL_FALSE, &camera->GetViewProjection()[0][0]);

	// Disable depthbuffer and enable blend
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	particle->BindTexture();

	// Bind the buffers holding the particles
	particle->Bind();

	// Draw the particles, send a draw call to the GPU
	particle->Draw();

	glDisable(GL_BLEND);
	glDepthMask(true);

	// Unbind the shader
	particleShader->UnBind();
}

void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, double counter)
{
	bloomBuffer->BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	pointLightPass->Bind();
	for (unsigned int i = 0; i < lights->GetNrOfLights(); i++)
	{
		pointLightPass->Update(*lights->GetTransform(i), *camera);
		lights->Draw(i);
	}
	pointLightPass->UnBind();
	glDisable(GL_CULL_FACE);
}

void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	blurShader->Bind();
	int timesToBlur = 10;
	bool horizontal = true;
	bool firstBlur = true;

	//GLuint loc = glGetUniformLocation(*blurShader->GetProgram(), "scene");

	for (int i = 0; i < timesToBlur; i++)
	{
		blurBuffers->BindForWriting(!horizontal);

		// First we read from the bloom fbo, to get a starting point of the blur. Then after that we will
		// blur the "blurred" texture over and over again, swapping between vertical and horizontal blurring.
		if (firstBlur == true)
		{
			bloomBuffer->BindForReadingBloomMap(0);
			blurShader->SendInt("horizontal", !horizontal);
			blurShader->SendInt("scene", 0);
		}
		else
		{
			blurBuffers->BindForReading(horizontal, 0);
			blurShader->SendInt("horizontal", !horizontal);

			blurShader->SendInt("scene", 0);
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

void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	finalFBO->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	finalBloomShader->Bind();

	bloomBuffer->BindForReadingDiffuse();
	finalBloomShader->SendInt("scene", 0);

	blurBuffers->BindForReading(1, 1);
	finalBloomShader->SendInt("bright", 1);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

}

void FinalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh * fullScreenTriangle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	finalFBO->BindForReading(0);

	finalShader->Bind();
	finalShader->SendInt("scene", 0);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);
}

void SendCameraLocationToGPU(GLuint cameraLocation, Camera *camera)
{
	glUniform3f(cameraLocation, camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);
}

void PrepareTexture(GLuint textureLoc, GLuint normalMapLoc)
{
	glUniform1i(textureLoc, 0);
	glUniform1i(normalMapLoc, 1);
}

void SetStartPositions(ObjectHandler * OH)
{

}

void GenerateMazeBitmaps(int height, int width)
{
	MazeGeneratePNG mazeGen(height, width);

	// Set_cell can be used to set "entrance and exit" etc
	//mazeGen.Set_cell(0, 1, mazeGen.path);
	//mazeGen.Set_cell(mazeHeight - 1, mazeWidth - 2, mazeGen.path);

	mazeGen.Generate();
	mazeGen.Draw_png();
}