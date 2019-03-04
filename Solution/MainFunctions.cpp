#include "MainFunctions.h"

void InitMazeGenerationShader(Shader * shader, Maze * maze)
{
	shader->InitiateMazeGenerationShader();

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("texture", 0);
	shader->SendInt("width", maze->GetMazeWidth());
	shader->SendInt("height", maze->GetMazeHeight());
	shader->SendVec2("translation", maze->GetTransform()->GetPos().x, maze->GetTransform()->GetPos().z);


	shader->SendInt("scaleUVXZ", maze->GetTransform()->GetScale().x);
	shader->SendInt("scaleUVY", maze->GetTransform()->GetScale().y);

	glm::vec2* drawOrder = maze->GetDrawOrder();
	for (int i = 0; i < maze->GetTileCount(); ++i)
	{
		shader->SendVec2(("drawOrder[" + std::to_string(i) + "]").c_str(), drawOrder[i].x, drawOrder[i].y);
	}

	shader->ValidateShaders();
}

void InitShadowShader(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms

	shader->ValidateShaders();
}

void InitGeometryPass(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();

	shader->ValidateShaders();
}

void InitMazeGeometryPass(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();

	shader->ValidateShaders();
}

void InitLightPass(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();
	shader->SendFloat("farPlane", (float)FAR_PLANE);
	shader->SendInt("gPosition", 0);
	shader->SendInt("gDiffuse", 1);
	shader->SendInt("gNormal", 2);
	shader->SendInt("gSpecularShininessHeight", 3);
	shader->SendInt("gAmbient", 4);
	shader->SendInt("shadowMap", 5);

	shader->ValidateShaders();
}

void InitParticleShader(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("particleTexture", 0);

	shader->ValidateShaders();
}

void InitPointLightPass(Shader * shader)
{

	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();

	shader->ValidateShaders();
}

void InitBlurShader(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);

	shader->ValidateShaders();
}

void InitFinalBloomShader(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);
	shader->SendInt("bright", 1);

	shader->ValidateShaders();
}

void InitFinalShader(Shader * shader)
{
	shader->InitiateShaders();

	// Set constant uniforms
	shader->Bind();
	shader->SendInt("scene", 0);

	shader->ValidateShaders();
}

void MazeGenerationPass(Shader * mazeGenerationShader, Maze * maze, Player * player)
{
	mazeGenerationShader->Bind();

	// Bind the mazeTexture (the colorcoded png)
	maze->BindTexture(0);

	// Send uniforms that needs to be updated each frame
	//wallShader->SendCameraLocation(player->GetCamera());
	mazeGenerationShader->SendVec3("cameraPos", player->GetCamera()->GetCameraPosition().x, player->GetCamera()->GetCameraPosition().y, player->GetCamera()->GetCameraPosition().z);



	// Draw the walls and store data with transform feedback
	maze->DrawMazeToBuffer();

	mazeGenerationShader->UnBind();
}

void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze * maze)
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
			OH->GetObject(j)->GetModel()->DrawMeshes(shadowShader);
		}
		
		// Draw Torch
		glm::mat4 worldMatrix = player->GetTorch()->GetTransform().GetWorldMatrix();
		shadowShader->SendMat4("WorldMatrix", worldMatrix);
		player->GetTorch()->GetModel()->DrawMeshes(shadowShader);

		// Same world matrix for walls and floor
		glm::mat4 mazeWorldMatrix = maze->GetTransform()->GetWorldMatrix();
		// Draw maze
		shadowShader->SendMat4("WorldMatrix", mazeWorldMatrix);
		maze->DrawMazeShadows();
	}

	shadowShader->UnBind();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glDisable(GL_DEPTH_TEST);
}

void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Shader *mazeGeometryPass, Player *player, ObjectHandler *OH, Maze * maze)
{
	geometryPass->Bind();

	gBuffer->BindForWriting();

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 worldMatrix = glm::mat4();
	// Update and Draw all objects
	for (unsigned int i = 0; i < OH->GetNrOfObjects(); i++)
	{
		worldMatrix = OH->GetObject(i)->GetTransform().GetWorldMatrix();
		geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
		geometryPass->SendMat4("WorldMatrix", worldMatrix);

		OH->GetObject(i)->Draw(geometryPass);
	}

	// Draw coins
	for (int i = 0; i < player->GetNrOfWorldCoins(); i++)
	{
		worldMatrix = player->GetWorldCoin(i)->GetTransform()->GetWorldMatrix();
		geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection()  * worldMatrix);
		geometryPass->SendMat4("WorldMatrix", worldMatrix);
		player->DrawCoin(i, geometryPass);
	}

	// Draw player torch
	worldMatrix = player->GetTorch()->GetTransform().GetWorldMatrix();
	geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
	geometryPass->SendMat4("WorldMatrix", worldMatrix);
	player->GetTorch()->Draw(geometryPass);

	geometryPass->UnBind();

	// Different geometry pass for the maze
	mazeGeometryPass->Bind();

	// Same world matrix for walls and floor
	glm::mat4 mazeWorldMatrix = maze->GetTransform()->GetWorldMatrix();
	
	// Draw Maze
	mazeGeometryPass->SendMat4("WorldMatrix", mazeWorldMatrix);
	mazeGeometryPass->SendMat4("VP", player->GetCamera()->GetViewProjection());
	mazeGeometryPass->SendFloat("Scale", maze->GetTransform()->GetScale().x);
	maze->BindMaterial(mazeGeometryPass);
	maze->DrawMaze();

	mazeGeometryPass->UnBind();
}

void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, GLuint *fullScreenTriangle, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera)
{
	lightPass->Bind();

	lights->SendLightsToShader(lightPass);
	lightPass->SendCameraLocation(camera);

	// Bloom buffer, write finalColor and brightness.
	bloomBuffer->BindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading(); // Binds texture slot 0,1,2,3,4
	shadowBuffer->BindForReading(5); // Binds texture slot 5

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(*fullScreenTriangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);

	lightPass->UnBind();
}

// This function draws particles to the screen.
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader)
{
	// Bind the correct shader
	particleShader->Bind();

	finalFBO->BindForWriting();

	// Send Uniforms
	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	particleShader->SendVec3("cameraRightWorld", camera->GetViewProjection()[0][0], camera->GetViewProjection()[1][0], camera->GetViewProjection()[2][0]);
	particleShader->SendVec3("cameraUpWorld", camera->GetViewProjection()[0][1], camera->GetViewProjection()[1][1], camera->GetViewProjection()[2][1]);
	particleShader->SendMat4("viewProjectionMatrix", camera->GetViewProjection());

	// Disable depthbuffer and enable blend
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	particle->GetTexture()->Bind(0);
	particleShader->SendInt("particleTexture", 0);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 3, &fullScreenTriangleData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 5, (void*) (sizeof(float) * 3));


	glBindVertexArray(0);

	return screenQuad;
}

void HandleEvents(Player* player, SoundHandler *winSound, SoundHandler * deathSound)
{
	EventHandler& EH = EventHandler::GetInstance();
	while (!EH.IsEmpty())
	{
		Event event = EH.GetEvent();

		if (event == EVENT_PLAYER_WIN)
		{
			//player->CenterPlayer();

			// IRRITERANDE LJUD
			winSound->SetPosition(player->GetPos());
			winSound->Play();
		}
		else if (event == EVENT_PLAYER_LOSE)
		{
			player->CenterPlayer();
			// IRRITERANDE LJUD
			//deathSound->Play();
		}
		else if (event == EVENT_PLAYER_DROPCOIN)
		{
			player->DropCoin();
		}
		else if (event == EVENT_PLAYER_TOSSCOIN)
		{
			player->TossCoin();
		}
	}
}

void SetMaxPatchVertices()
{
	GLint MaxPatchVertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
	printf("Max supported patch vertices %d\n", MaxPatchVertices);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
}
