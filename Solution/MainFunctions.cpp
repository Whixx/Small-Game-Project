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


	shader->SendFloat("scaleUVXZ", maze->GetTransform()->GetScale().x);
	shader->SendFloat("scaleUVY", maze->GetTransform()->GetScale().y);

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
	shader->SendInt("gEmissive", 4);
	shader->SendInt("gAmbient", 5);
	shader->SendInt("shadowMap", 6);

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

void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze * maze, Exit * exit)
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

		// Draw landmarks
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

		// Draw exit
		worldMatrix = exit->GetWorldMatrix();
		shadowShader->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
		shadowShader->SendMat4("WorldMatrix", worldMatrix);
		if (!maze->IsExitOpen())
		{
			exit->DrawClosed(shadowShader);
		}
		else
		{
			exit->DrawOpen(shadowShader);
		}

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

void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Shader *mazeGeometryPass, Player *player, ObjectHandler *OH, Maze * maze, Minotaur * minotaur, Exit * exit)
{
	geometryPass->Bind();

	gBuffer->BindForWriting();

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 worldMatrix;

	// Draw landmarks
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

	// Draw minotaur
	worldMatrix = minotaur->GetTransform().GetWorldMatrix();
	geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
	geometryPass->SendMat4("WorldMatrix", worldMatrix);
	minotaur->Draw(geometryPass);

	// Draw player torch
	worldMatrix = player->GetTorch()->GetTransform().GetWorldMatrix();
	geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
	geometryPass->SendMat4("WorldMatrix", worldMatrix);
	player->GetTorch()->Draw(geometryPass);

	// Draw keystones
	for (int i = 0; i < maze->GetNrOfKeystones(); i++)
	{
		worldMatrix = maze->GetKeystoneTransform(i)->GetWorldMatrix();
		geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
		geometryPass->SendMat4("WorldMatrix", worldMatrix);
		maze->DrawKeystone(i, geometryPass);
	}

	// Draw exit
	worldMatrix = exit->GetWorldMatrix();
	geometryPass->SendMat4("transformationMatrix", player->GetCamera()->GetViewProjection() * worldMatrix);
	geometryPass->SendMat4("WorldMatrix", worldMatrix);
	if (!maze->IsExitOpen())
	{
		exit->DrawClosed(geometryPass);
	}
	else
	{
		exit->DrawOpen(geometryPass);
	}

	geometryPass->UnBind();

	// Different geometry pass for the maze
	mazeGeometryPass->Bind();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Same world matrix for walls and floor
	glm::mat4 mazeWorldMatrix = maze->GetTransform()->GetWorldMatrix();
	
	// Draw Maze
	mazeGeometryPass->SendMat4("WorldMatrix", mazeWorldMatrix);
	mazeGeometryPass->SendMat4("VP", player->GetCamera()->GetViewProjection());
	mazeGeometryPass->SendFloat("Scale", maze->GetTransform()->GetScale().x);
	maze->BindMaterial(mazeGeometryPass);
	maze->DrawMaze();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mazeGeometryPass->UnBind();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DRLightPass(GBuffer *gBuffer, FinalFBO * finalFBO, GLuint *fullScreenTriangle, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera)
{
	lightPass->Bind();

	lights->SendLightsToShader(lightPass);
	lightPass->SendCameraLocation(camera);

	// Bloom buffer, write finalColor and brightness.
	finalFBO->BindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading(); // Binds texture slot 0,1,2,3,4,5
	shadowBuffer->BindForReading(6); // Binds texture slot 6

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

std::vector<std::vector<int>> GenerateMazePNG(int height, int width)
{
	MazeGeneratePNG mazeGen(height, width);

	mazeGen.Generate();
	mazeGen.GenerateExit();
	mazeGen.DrawPNG();

	return mazeGen.GetGrid();
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

void HandleEvents(Player* player, Maze * maze, Sound *winSound, Sound * deathSound, Sound * minotaurGrowlSound, Minotaur * minotaur)
{
	EventHandler& EH = EventHandler::GetInstance();
	while (!EH.IsEmpty())
	{
		Event event = EH.GetEvent();

		if (event == EVENT_PLAYER_WIN)
		{
			//player->CenterPlayer();

			winSound->SetPosition(player->GetPos());
			winSound->Play();
		}
		else if (event == EVENT_PLAYER_LOSE)
		{
			//player->CenterPlayer();
			deathSound->Play();
		}
		else if (event == EVENT_PLAYER_DROPCOIN)
		{
			player->DropCoin();
		}
		else if (event == EVENT_PLAYER_TOSSCOIN)
		{
			player->TossCoin();
		}
		else if (event == EVENT_PLAYER_PICKUPCOIN)
		{
			player->PickUpCoin();
		}
		else if (event == EVENT_MAZE_KEYSTONE_PRESSED)
		{
			glm::vec3 keystonePosition = player->GetCamera()->GetCameraPosition();
			// The function will check which keystone was pressed
			if (maze->ActivateKeystone(player->GetPos(), minotaurGrowlSound))
			{
				keystonePosition = maze->TransformToMazeCoords(keystonePosition);

				minotaur->reactToSound(keystonePosition);
			}
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

void CreateLandmarks(ObjectHandler * OH, Maze * maze)
{
	int altar = OH->CreateObject("Models/Landmarks/Altar/Blood_Altar.obj");
	OH->GetObject(altar)->GetScale() = glm::vec3(20.0f);
	OH->GetObject(altar)->GetRot().z += glm::radians(180.0f);

	int bottle = OH->CreateObject("Models/Landmarks/Broken_bottle/broken_bottle.obj");
	OH->GetObject(bottle)->GetScale() = glm::vec3(0.8f);

	int bottle2 = OH->CreateObject("Models/Landmarks/Broken_bottle/broken_bottle.obj");
	OH->GetObject(bottle2)->GetScale() = glm::vec3(0.8f);

	int bottle3 = OH->CreateObject("Models/Landmarks/Broken_bottle/broken_bottle.obj");
	OH->GetObject(bottle3)->GetScale() = glm::vec3(0.8f);

	int banner = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner)->GetScale() = glm::vec3(0.3f);

	int banner1 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner1)->GetScale() = glm::vec3(0.3f);

	int banner2 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner2)->GetScale() = glm::vec3(0.3f);

	int banner3 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner3)->GetScale() = glm::vec3(0.3f);

	int banner4 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner4)->GetScale() = glm::vec3(0.3f);

	int banner5 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner5)->GetScale() = glm::vec3(0.3f);

	int banner6 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner6)->GetScale() = glm::vec3(0.3f);

	int banner7 = OH->CreateObject("Models/Landmarks/Banner/Banner.obj");
	OH->GetObject(banner7)->GetScale() = glm::vec3(0.3f);

	int vase = OH->CreateObject("Models/Landmarks/Vase/vase.obj");
	OH->GetObject(vase)->GetScale() = glm::vec3(1.0f);

	int vase2 = OH->CreateObject("Models/Landmarks/Vase/vase.obj");
	OH->GetObject(vase2)->GetScale() = glm::vec3(0.8f);

	int vase3 = OH->CreateObject("Models/Landmarks/Vase/vase.obj");
	OH->GetObject(vase3)->GetScale() = glm::vec3(0.9f);

	int shroom = OH->CreateObject("Models/Landmarks/Shroom/shroom.obj");
	OH->GetObject(shroom)->GetScale() = glm::vec3(0.2f);

	int shroom2 = OH->CreateObject("Models/Landmarks/Shroom/shroom.obj");
	OH->GetObject(shroom2)->GetScale() = glm::vec3(0.2f);

	int shroom3 = OH->CreateObject("Models/Landmarks/Shroom/shroom.obj");
	OH->GetObject(shroom3)->GetScale() = glm::vec3(0.1f);

	int shroom4 = OH->CreateObject("Models/Landmarks/Shroom/shroom.obj");
	OH->GetObject(shroom4)->GetScale() = glm::vec3(0.1f);

	int skeletonPile = OH->CreateObject("Models/Landmarks/Skeleton_pile/skeleton_pile.obj");
	OH->GetObject(skeletonPile)->GetScale() = glm::vec3(0.7f);
	OH->GetObject(skeletonPile)->GetRot().x += glm::radians(180.0f);

	int skeletonPile2 = OH->CreateObject("Models/Landmarks/Skeleton_pile/skeleton_pile.obj");
	OH->GetObject(skeletonPile2)->GetScale() = glm::vec3(0.7f);
	OH->GetObject(skeletonPile2)->GetRot().x += glm::radians(180.0f);

	int stone = OH->CreateObject("Models/Landmarks/Stone/stone.obj");
	OH->GetObject(stone)->GetScale() = glm::vec3(0.02f);

	int stone2 = OH->CreateObject("Models/Landmarks/Stone/stone.obj");
	OH->GetObject(stone2)->GetScale() = glm::vec3(0.023f);

	int stone3 = OH->CreateObject("Models/Landmarks/Stone/stone.obj");
	OH->GetObject(stone3)->GetScale() = glm::vec3(0.015f);

	int stone4 = OH->CreateObject("Models/Landmarks/Stone/stone.obj");
	OH->GetObject(stone4)->GetScale() = glm::vec3(0.01f);

	// Make sure that every object has a unique tile
	for (int i = 0; i < OH->GetNrOfObjects(); i++)
	{
	RESTART:
		bool found = true;
		OH->GetObject(i)->GetPos() = maze->GetRandomFloorPos();

		// Is this object's position similar to a keystone's position?
		for (int k = 0; k < maze->GetNrOfKeystones(); k++)
		{
			// Check right wall
			glm::vec3 translate = OH->GetObject(i)->GetPos();
			translate.x += 1.0f;

			if (floor(translate.x) == floor(maze->GetKeystoneTransform(i)->GetPos().x))
			{
				// Get a new random position and restart the search
				goto RESTART;
			}

			// Check left wall
			translate = OH->GetObject(i)->GetPos();
			translate.x -= 1.0f;

			if (floor(translate.x) == floor(maze->GetKeystoneTransform(i)->GetPos().x))
			{
				// Get a new random position and restart the search
				goto RESTART;
			}

			// Check lower wall
			translate = OH->GetObject(i)->GetPos();

			if (floor(translate.z) == floor(maze->GetKeystoneTransform(i)->GetPos().z))
			{
				// Get a new random position and restart the search
				goto RESTART;
			}

			// Check upper wall
			translate = OH->GetObject(i)->GetPos();
			translate.z -= 1.0f;

			if (floor(translate.z) == floor(maze->GetKeystoneTransform(i)->GetPos().z))
			{
				// Get a new random position and restart the search
				goto RESTART;
			}
		}

		while (found == true)
		{
			for (int j = 0; j < OH->GetNrOfObjects(); j++)
			{
				// Skip if it matches with itself
				if (i == j)
				{
					continue;
				}

				// Is this object's position similar to another's?
				if (floor(OH->GetObject(i)->GetPos()) == floor(OH->GetObject(j)->GetPos()))
				{
					// Get a new random position and restart the search
					goto RESTART;
				}
				else 
				{
					found = false;
				}
			}
		}

		// Turn and place the object towards the wall
		if (i == vase || i == vase2 || i == vase3 || i == shroom || i == shroom2 || i == shroom3 || i == shroom4)
		{
			float tessOffset = 0.15f * maze->GetTransform()->GetScale().x;
			OH->GetObject(i)->GetPos() = floor(OH->GetObject(i)->GetPos());
			OH->GetObject(i)->GetPos().x += tessOffset;
			OH->GetObject(i)->GetPos().z += tessOffset;

			glm::vec3 pos = OH->GetObject(i)->GetPos();

			if (maze->IsWallAtWorld(pos.x, pos.z + 1) == false && maze->IsWallAtWorld(pos.x, pos.z - 1) == false)
			{
				OH->GetObject(i)->GetRot().y += glm::radians(90.0f);
			}
		}
		// Turn, lift and place the banner towards the walls
		else if (i == banner || i == banner1
			|| i == banner2 || i == banner3 
			|| i == banner4 || i == banner5
			|| i == banner6 || i == banner7)
		{
			float tessOffset = 0.05f * maze->GetTransform()->GetScale().x;
			float lift = 0.13f * maze->GetTransform()->GetScale().y;
			glm::vec3 pos = OH->GetObject(i)->GetPos();

			if (maze->IsWallAtWorld(pos.x, pos.z + 1) == true)
			{
				OH->GetObject(i)->GetPos().z = ceil(OH->GetObject(i)->GetPos().z);
				OH->GetObject(i)->GetPos().z -= tessOffset;
			}
			else if (maze->IsWallAtWorld(pos.x, pos.z - 1) == true)
			{
				OH->GetObject(i)->GetPos().z = floor(OH->GetObject(i)->GetPos().z);
				OH->GetObject(i)->GetPos().z += tessOffset;
				OH->GetObject(i)->GetRot().y += glm::radians(0.0f);
			}
			else if (maze->IsWallAtWorld(pos.x + 1, pos.z) == true)
			{
				OH->GetObject(i)->GetPos().x = ceil(OH->GetObject(i)->GetPos().x);
				OH->GetObject(i)->GetPos().x -= tessOffset;
				OH->GetObject(i)->GetRot().y += glm::radians(-90.0f);
			}
			else if (maze->IsWallAtWorld(pos.x - 1, pos.z) == true)
			{
				OH->GetObject(i)->GetPos().x = floor(OH->GetObject(i)->GetPos().x);
				OH->GetObject(i)->GetPos().x += tessOffset;
				OH->GetObject(i)->GetRot().y += glm::radians(+90.0f);
			}
			else
			{
				goto RESTART;
			}

			OH->GetObject(i)->GetPos().y += lift;
		}
		else
		{
			// Give the object a random rotation
			OH->GetObject(i)->GetRot().y = glm::radians((float)(rand() % 360));
		}

#ifdef DEBUG
		printf("Object %d, Position: X: %f, Y: %f, Z: %f\n", i, OH->GetObject(i)->GetPos().x, OH->GetObject(i)->GetPos().y, OH->GetObject(i)->GetPos().z);
#endif // DEBUG
	}
}

void ResetLandmarks(ObjectHandler * OH, Maze * maze)
{
	OH->~ObjectHandler();
	CreateLandmarks(OH, maze);
}
