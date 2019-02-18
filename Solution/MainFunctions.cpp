#include "MainFunctions.h"

void InitShadowShader(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms

	shader->ValidateShaders();
}

void InitGeometryPass(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();

	shader->ValidateShaders();
}

void InitLightPass(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("gPosition", 0);
	shader->SendInt("gDiffuse", 1);
	shader->SendInt("gNormal", 2);
	shader->SendInt("shadowMap", 3);

	shader->ValidateShaders();
}

void InitParticleShader(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("particleTexture", 0);

	shader->ValidateShaders();
}

void InitPointLightPass(Shader * shader)
{
	shader->InitiateShaders(true);

	// Set constant uniforms
	shader->Bind();

	shader->ValidateShaders();
}

void InitBlurShader(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);

	shader->ValidateShaders();
}

void InitFinalBloomShader(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);
	shader->SendInt("bright", 1);

	shader->ValidateShaders();
}

void InitFinalShader(Shader * shader)
{
	shader->InitiateShaders(false);

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);

	shader->ValidateShaders();
}

void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player)
{
	shadowShader->Bind();
	glEnable(GL_DEPTH_TEST);
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
			shadowShader->SendMat4(("shadowMatrices[" + std::to_string(j) + "]").c_str(), shadowTransforms[j]);
		}
		shadowShader->SendFloat("farPlane", (float)FAR_PLANE);
		shadowShader->SendVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

		for (unsigned int j = 0; j < OH->GetNrOfObjects(); j++)
		{
			glm::mat4 worldMatrix = OH->GetObject(j)->GetTransform().GetWorldMatrix();
			shadowShader->SendMat4("WorldMatrix", worldMatrix);
			OH->GetObject(j)->Draw(shadowShader);
		}

		// Draw player torch
		glm::mat4 worldMatrix = player->GetTorch()->GetTransform().GetWorldMatrix();
		shadowShader->SendMat4("WorldMatrix", worldMatrix);
		//player->GetTorch()->BindTexture();
		//player->GetTorch()->Draw(shadowShader);
	}

	shadowShader->UnBind();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glDisable(GL_DEPTH_TEST);
}

void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Player *player, ObjectHandler *OH)
{
	geometryPass->Bind();

	geometryPass->SendCameraLocation(player->GetCamera());

	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// Update and Draw all objects
	for (unsigned int i = 0; i < OH->GetNrOfObjects(); i++)
	{
		geometryPass->SendFloat("illuminated", 1.0f);
		glm::mat4 worldMatrix = OH->GetObject(i)->GetTransform().GetWorldMatrix();
		geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
		geometryPass->SendMat4("WorldMatrix", worldMatrix);
		OH->GetObject(i)->Draw(geometryPass);
	}

	// Draw player torch
	geometryPass->SendFloat("illuminated", 3.0f);
	glm::mat4 worldMatrix = player->GetTorch()->GetTransform().GetWorldMatrix();
	geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
	geometryPass->SendMat4("WorldMatrix", worldMatrix);
	player->GetTorch()->Draw(geometryPass);
	
	geometryPass->UnBind();
}

void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, GLuint *fullScreenTriangle, GLuint *program, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera)
{
	lightPass->Bind();

	lights->SendLightsToShader(lightPass);
	lightPass->SendCameraLocation(camera);
	lightPass->SendFloat("farPlane", (float)FAR_PLANE);

	// Bloom buffer, write finalColor and brightness.
	bloomBuffer->BindForWriting();
	bloomBuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading(); // Binds texture slot 0,1,2
	shadowBuffer->BindForReading(3); // Binds texture slot 3

	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(*fullScreenTriangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);

	lightPass->UnBind();
}

// This function draws particles to the screen.
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position)
{
	// Bind the correct shader
	particleShader->Bind();

	finalFBO->BindForWriting();

	// Send Uniforms
	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	particleShader->SendVec3("cameraRightWorld", camera->GetViewProjection()[0][0], camera->GetViewProjection()[1][0], camera->GetViewProjection()[2][0]);
	particleShader->SendVec3("cameraUpWorld", camera->GetViewProjection()[0][1], camera->GetViewProjection()[1][1], camera->GetViewProjection()[2][1]);
	particleShader->SendMat4("viewProjectionMatrix", camera->GetViewProjection());


	// Create new particles
	particle->GenerateParticles(deltaTime, position);

	// Simulate all the particles
	particle->SimulateParticles(camera->GetCameraPosition(), deltaTime);

	// Update the buffers holding the particles
	particle->Update();



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

void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, Model *renderModel)
{
	bloomBuffer->BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	pointLightPass->Bind();
	for (unsigned int i = 0; i < lights->GetNrOfLights(); i++)
	{
		glm::mat4 worldMatrix = lights->GetTransform(i)->GetWorldMatrix();
		pointLightPass->SendMat4("transformationMatrix", camera->GetViewProjection() * worldMatrix);

		renderModel->Draw(pointLightPass);
	}
	pointLightPass->UnBind();
	glDisable(GL_CULL_FACE);
}

void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle)
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
		}
		else
		{
			blurBuffers->BindForReading(horizontal, 0);
			blurShader->SendInt("horizontal", !horizontal);
		}

		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(*fullScreenTriangle);
		glDrawArrays(GL_TRIANGLES, 0, 3);;
		glEnable(GL_DEPTH_TEST);

		horizontal = !horizontal;
		if (firstBlur)
		{
			firstBlur = false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle)
{
	finalFBO->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	finalBloomShader->Bind();

	bloomBuffer->BindForReadingDiffuse();
	blurBuffers->BindForReading(1, 1);

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(*fullScreenTriangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);

}

void FinalPass(FinalFBO * finalFBO, Shader * finalShader, GLuint * fullScreenTriangle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	finalFBO->BindForReading(0);

	finalShader->Bind();

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(*fullScreenTriangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);
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

GLuint CreateScreenQuad()
{
	// https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/

	float fullScreenTriangleData[] = {
		-1.0, 3.0, 0.0, 0.0, 2.0,
		-1.0, -1.0, 0.0, 0.0, 0.0,
		3.0, -1.0, 0.0, 2.0, 0.0
	};
	GLuint screenQuad;

	glGenVertexArrays(1, &screenQuad);
	glBindVertexArray(screenQuad);

	GLuint quadBuffer;
	glGenBuffers(1, &quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullScreenTriangleData), fullScreenTriangleData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	return screenQuad;
}
