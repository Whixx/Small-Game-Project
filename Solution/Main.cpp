#define STB_IMAGE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#include <glew\glew.h>
#undef main

// Uses stb_image


//
#include "MazeGeneratePNG.h"
#include "Texture.h"


#include "Shader.h"
#include "ObjectHandler.h"
#include "PointLight.h"
#include "ShadowMap.h"
#include "Bloom.h"
#include "Blur.h"
#include "GBuffer.h"
#include "FinalFBO.h"
#include "Player.h"
#include "Particle.h"
#include "InputHandler.h"

#include "MainFunctions.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	Display display;

	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(display.GetWindow(), InputHandler::Key_callback);

	InputHandler IH;

	// height and width must be odd numbers else the resulting maze will be off
	// inside the maze class the image will be made in to an even power of two number (ATM hardcoded 64) for use in shaders
	GenerateMazeBitmaps(63, 63); // Creates maze.png + maze_d.png

	Maze maze;
	maze.LoadMaze("MazePNG/mazeColorCoded.png");

	//=========================== Creating Shaders ====================================//
	Shader wallShader;
	wallShader.CreateShader(".\\wallShader.vs", GL_VERTEX_SHADER);
	wallShader.CreateShader(".\\wallShader.gs", GL_GEOMETRY_SHADER);
	wallShader.CreateShader(".\\wallShader.fs", GL_FRAGMENT_SHADER);

	Shader floorShader;
	floorShader.CreateShader(".\\floorShader.vs", GL_VERTEX_SHADER);
	floorShader.CreateShader(".\\floorShader.gs", GL_GEOMETRY_SHADER);
	floorShader.CreateShader(".\\floorShader.fs", GL_FRAGMENT_SHADER);
	
	Shader shadowShader;
	shadowShader.CreateShader(".\\shadowShader.vs", GL_VERTEX_SHADER);
	shadowShader.CreateShader(".\\shadowShader.gs", GL_GEOMETRY_SHADER);
	shadowShader.CreateShader(".\\shadowShader.fs", GL_FRAGMENT_SHADER);

	Shader geometryPass;
	geometryPass.CreateShader(".\\geometryPass.vs", GL_VERTEX_SHADER);
	geometryPass.CreateShader(".\\geometryPass.gs", GL_GEOMETRY_SHADER);
	geometryPass.CreateShader(".\\geometryPass.fs", GL_FRAGMENT_SHADER);
	
	Shader lightPass;
	lightPass.CreateShader(".\\lightPass.vs", GL_VERTEX_SHADER);
	lightPass.CreateShader(".\\lightPass.fs", GL_FRAGMENT_SHADER);

	Shader particleShader;
	particleShader.CreateShader(".\\particleShader.vs", GL_VERTEX_SHADER);
	particleShader.CreateShader(".\\particleShader.fs", GL_FRAGMENT_SHADER);

	Shader pointLightPass;
	pointLightPass.CreateShader(".\\pointLightShader.vs", GL_VERTEX_SHADER);
	pointLightPass.CreateShader(".\\pointLightShader.fs", GL_FRAGMENT_SHADER);

	Shader blurShader;
	blurShader.CreateShader(".\\blurShader.vs", GL_VERTEX_SHADER);
	blurShader.CreateShader(".\\blurShader.fs", GL_FRAGMENT_SHADER);

	Shader finalBloomShader;
	finalBloomShader.CreateShader(".\\finalBloomShader.vs", GL_VERTEX_SHADER);
	finalBloomShader.CreateShader(".\\finalBloomShader.fs", GL_FRAGMENT_SHADER);

	Shader finalShader;
	finalShader.CreateShader(".\\finalShader.vs", GL_VERTEX_SHADER);
	finalShader.CreateShader(".\\finalShader.fs", GL_FRAGMENT_SHADER);

	InitWallShader(&wallShader, &maze);
	InitFloorShader(&floorShader, &maze);
	InitShadowShader(&shadowShader);
	InitGeometryPass(&geometryPass);
	InitLightPass(&lightPass);
	InitParticleShader(&particleShader);
	InitPointLightPass(&pointLightPass);
	InitBlurShader(&blurShader);
	InitFinalBloomShader(&finalBloomShader);
	InitFinalShader(&finalShader);

	//=================================================================================//

	//=========================== Buffers ====================================//
	ShadowMap shadowMap(SHADOW_WIDTH, SHADOW_HEIGHT);
	GBuffer gBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	BloomBuffer bloomBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	BlurBuffer blurBuffers(SCREEN_WIDTH, SCREEN_HEIGHT);
	FinalFBO finalFBO(SCREEN_WIDTH, SCREEN_HEIGHT);

	//=========================== Creating Objects ====================================//

	// Temp texture for the mazeWalls
	Texture brickTexture("Textures/brickwall.jpg", "NormalMaps/brickwall_normal.jpg");

	// Create Lights
	PointLightHandler lights;	// use .CreateLight()

	Mesh groundMesh;
	Mesh torchMesh;
	Texture torchTexture("Textures/torch.png", "NormalMaps/torch_normal.png");


	// Sound engine that plays all the sounds, pass reference to classes that will use sound with enginePtr
	SoundEngine soundEngine;
	irrklang::ISoundEngine* enginePtr = soundEngine.GetEngine();

	// minotaur sound test stuff
	glm::vec3 newPosition;
	newPosition.y = 0.0;
	SoundHandler minotaurGrowl("Sounds/minotaurgrowl.wav", false, enginePtr);
	SoundHandler minotaurFootStep("Sounds/minotaurstep.ogg", false, enginePtr);
	minotaurGrowl.SetMinDistance(0.5);


	float playerHeight = 1.8f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, &torchMesh, &torchTexture, &maze, enginePtr, &lights);
	player.SetPlayerSpeed(2.0f);
	player.CenterPlayer(); //Space to return to origin

	Texture groundTexture("Textures/ground.png", "NormalMaps/ground_normal.png");

	ObjectHandler OH;

	//TODO: Byta ground.png till floor.png
	
	Model lightSphereModel("Models/Ball/ball.obj");
	GLuint screenQuad = CreateScreenQuad();
	Particle particle;
	Texture particleTexture("Textures/particle.png");
	particle.SetTexture(&particleTexture);
	// Initiate timer
	double currentTime = 0;
	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double constLastTime = 0;
	int nrOfFrames = 0;

	while (!display.IsWindowClosed())
	{
		// Calculate DeltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - constLastTime;

		// Measure fps
		nrOfFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			// If last print was more than 1 sec ago, print and reset timer
			display.SetTitle("FPS: " + to_string((int)((double)nrOfFrames)));
			nrOfFrames = 0;
			lastTime += 1.0;
		}


		// ================== EVENTS ==================

		// Update movement 
		IH.MouseControls(&display, &player, deltaTime);
		IH.KeyboardControls(&display, &player, deltaTime);

		// ================== UPDATE ==================

		// Update player
		player.Update(deltaTime);

		OH.UpdateAllObjects(deltaTime);

		//lights.GetTransform(0)->GetPos() = glm::vec3(player.GetTorch()->GetPos().x, player.GetTorch()->GetPos().y + 1.5f, player.GetTorch()->GetPos().z);
		lights.UpdateShadowTransform(0);

		// Update particles
		particle.Update(deltaTime, player.GetCamera(), player.GetTorch()->GetPos());

		// update sound engine with position and view direction
		soundEngine.Update(player.GetCamera()->GetCameraPosition(), player.GetCamera()->GetForwardVector());

		//// moving minotaur sound test
		//newPosition.x = sinf(glfwGetTime() * 0.2 * 3.15) * 5.0f;
		//newPosition.z = cosf(glfwGetTime() * 0.2 * 3.15) * 5.0f;
		//minotaurGrowl.SetPosition(newPosition);
		//minotaurGrowl.Play();
		//minotaurFootStep.SetPosition(newPosition);
		//minotaurFootStep.Play();


		// ================== DRAW ==================

		// Here the walls are created and stored in a buffer with transform feedback
		WallPass(&wallShader, &maze, &player);

		// Here the floor is created and stored in a buffer with transform feedback
		FloorPass(&floorShader, &maze, &player);

		// Here a cube map is calculated and stored in the shadowMap FBO
		ShadowPass(&shadowShader, &OH, &lights, &shadowMap, &player, &maze);
		
		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, &geometryPass, &player, &OH, &maze, &brickTexture, &groundTexture);
		
		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &bloomBuffer, &screenQuad, &lightPass, &shadowMap, &lights, player.GetCamera());

		// Copy the depth from the gBuffer to the bloomBuffer
		bloomBuffer.CopyDepth(SCREEN_WIDTH, SCREEN_HEIGHT, gBuffer.GetFBO());

		// Draw lightSpheres
		#ifdef DEBUG
			LightSpherePass(&pointLightPass, &bloomBuffer, &lights, player.GetCamera(), &lightSphereModel);
		#endif
			
		// Blur the bright texture
		BlurPass(&blurShader, &bloomBuffer, &blurBuffers, &screenQuad);

		// Combine the bright texture and the scene and store the Result in FinalFBO.
		FinalBloomPass(&finalBloomShader, &finalFBO, &bloomBuffer, &blurBuffers, &screenQuad);

		// Copy the depth from the bloomBuffer to the finalFBO
		finalFBO.CopyDepth(SCREEN_WIDTH, SCREEN_HEIGHT, bloomBuffer.GetFBO());

		// Draw particles to the FinalFBO
		//ParticlePass(&finalFBO, &player.GetTorch()->GetParticle(), player.GetCamera(), &particleShader, deltaTime, player.GetTorch()->GetFirePos());
		ParticlePass(&finalFBO, &particle, player.GetCamera(), &particleShader);

		// Render everything
		FinalPass(&finalFBO, &finalShader, &screenQuad);



		// ================== POST DRAW ==================
		display.SwapBuffers(SCREEN_WIDTH, SCREEN_HEIGHT);

		constLastTime = currentTime;
	}
	glfwTerminate();
	return 0;
}
