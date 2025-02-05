﻿// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#include <glew\glew.h>
#undef main

// Uses stb_image

#include "MainFunctions.h"


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	srand(time(NULL));
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	Display display;

	bool paused = true;
	bool startMenu = true;		// true = startmenu | false = in game menu

	// use GLFW_CURSOS_NORMAL when starting the game "paused"
	//glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetKeyCallback(display.GetWindow(), InputHandler::Key_callback);
	glfwSetMouseButtonCallback(display.GetWindow(), InputHandler::mouse_button_callback);

	EventHandler& EH = EventHandler::GetInstance();
	InputHandler IH;
	MaterialHandler& MH = MaterialHandler::GetInstance(); // Singleton

	// height and width must be odd numbers, more specifically: a power of two number - 1 (example 15, 31, 63 etc)
	// inside the maze class the image will be made in to an even power of two number for use in shaders
	std::vector<std::vector<int>> mazeGrid = GenerateMazePNG(31, 31);

	// Sound engine that plays all the sounds, pass reference to classes that will use sound with enginePtr
	SoundEngine soundEngine;
	irrklang::ISoundEngine* enginePtr = soundEngine.GetEngine();

	Maze maze(enginePtr);
	Exit exit = *maze.GetExit();

	// Create Lights
	PointLightHandler lights;	// use .CreateLight()

	// PLAYER AND MINOTAUR
	Minotaur minotaur(enginePtr, mazeGrid, &maze);
	float playerHeight = 1.4f;

	float torchSize = 0.02f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, &maze, enginePtr, &lights, torchSize, &minotaur);

	//=========================== Creating Shaders ====================================//

	// MaxVertices supported by the hardware
	SetMaxPatchVertices();

	Shader mazeGenerationShader;
	mazeGenerationShader.CreateShader("Shaders\\mazeGenerationShader.vs", GL_VERTEX_SHADER);
	mazeGenerationShader.CreateShader("Shaders\\mazeGenerationShader.gs", GL_GEOMETRY_SHADER);
	mazeGenerationShader.CreateShader("Shaders\\mazeGenerationShader.fs", GL_FRAGMENT_SHADER);	

	Shader shadowShader;
	shadowShader.CreateShader("Shaders\\shadowShader.vs", GL_VERTEX_SHADER);
	shadowShader.CreateShader("Shaders\\shadowShader.gs", GL_GEOMETRY_SHADER);
	shadowShader.CreateShader("Shaders\\shadowShader.fs", GL_FRAGMENT_SHADER);
	
	Shader shadowAnimationShader;
	shadowAnimationShader.CreateShader("Shaders\\shadowAnimationShader.vs", GL_VERTEX_SHADER);
	shadowAnimationShader.CreateShader("Shaders\\shadowAnimationShader.gs", GL_GEOMETRY_SHADER);
	shadowAnimationShader.CreateShader("Shaders\\shadowAnimationShader.fs", GL_FRAGMENT_SHADER);

	Shader geometryPass;
	geometryPass.CreateShader("Shaders\\geometryPass.vs", GL_VERTEX_SHADER);
	geometryPass.CreateShader("Shaders\\geometryPass.fs", GL_FRAGMENT_SHADER);

	Shader mazeGeometryPass;
	mazeGeometryPass.CreateShader("Shaders\\mazeGeometryPass.vs", GL_VERTEX_SHADER);
	mazeGeometryPass.CreateShader("Shaders\\mazeGeometryPass.cs", GL_TESS_CONTROL_SHADER);
	mazeGeometryPass.CreateShader("Shaders\\mazeGeometryPass.es", GL_TESS_EVALUATION_SHADER);
	mazeGeometryPass.CreateShader("Shaders\\mazeGeometryPass.fs", GL_FRAGMENT_SHADER);

	Shader animationPass;
	animationPass.CreateShader("Shaders\\animationPass.vs", GL_VERTEX_SHADER);
	animationPass.CreateShader("Shaders\\animationPass.fs", GL_FRAGMENT_SHADER);

	Shader lightPass;
	lightPass.CreateShader("Shaders\\lightPass.vs", GL_VERTEX_SHADER);
	lightPass.CreateShader("Shaders\\lightPass.fs", GL_FRAGMENT_SHADER);

	Shader particleShader;
	particleShader.CreateShader("Shaders\\particleShader.vs", GL_VERTEX_SHADER);
	particleShader.CreateShader("Shaders\\particleShader.fs", GL_FRAGMENT_SHADER);

	Shader finalShader;
	finalShader.CreateShader("Shaders\\finalShader.vs", GL_VERTEX_SHADER);
	finalShader.CreateShader("Shaders\\finalShader.fs", GL_FRAGMENT_SHADER);

	Shader coinUIShader;
	coinUIShader.CreateShader("Shaders\\coinUIShader.vs", GL_VERTEX_SHADER);
	coinUIShader.CreateShader("Shaders\\coinUIShader.fs", GL_FRAGMENT_SHADER);

	Shader button2DShader;
	button2DShader.CreateShader("Shaders\\button2DShader.vs", GL_VERTEX_SHADER);
	button2DShader.CreateShader("Shaders\\button2DShader.fs", GL_FRAGMENT_SHADER);

	InitMazeGenerationShader(&mazeGenerationShader, &maze);
	InitShadowShader(&shadowShader);

	InitShadowAnimationShader(&shadowAnimationShader);

	InitGeometryPass(&geometryPass);
	InitMazeGeometryPass(&mazeGeometryPass);
	InitAnimationPass(&animationPass);
	InitLightPass(&lightPass);
	InitParticleShader(&particleShader);
	InitFinalShader(&finalShader);
	InitCoinUIShader(&coinUIShader, &player);
	InitButton2DShader(&button2DShader);

	//=================================================================================//

	//=========================== Buffers ====================================//
	ShadowMap shadowMap(SHADOW_WIDTH, SHADOW_HEIGHT);
	GBuffer gBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	FinalFBO finalFBO(SCREEN_WIDTH, SCREEN_HEIGHT);

	//=========================== Creating Objects ====================================//

	Sound winSound("Sounds/winSound.mp3", false, enginePtr);
	Sound deathSound("Sounds/death.wav", false, enginePtr);
	minotaur.Initialize();
	
	Model lightSphereModel("Models/Ball/ball.obj");

	// 2D quads
	ClipSpaceQuad fullScreenQuad;
	ClipSpaceQuad coinInterfaceQuad(glm::vec2(-0.4, -0.4), 1.0f, 0.05f, false, "Textures/UI/coins.png");

	// MENU STUFF
	Menu buttonHandler;
	int startMenuBackground = buttonHandler.AddButton(glm::vec2(0.0f, 0.0f), 1.0f, 1.0f, "Textures/Menu/startmenu.jpg", MENU_START);	// Background for start menu
	int startButton = buttonHandler.AddButton(glm::vec2(-0.6f, 0.0f), 0.35f, 0.35f, "Textures/Menu/play.png", MENU_START);
	int quitButton = buttonHandler.AddButton(glm::vec2(0.6f, 0.0f), 0.35f, 0.35f, "Textures/Menu/quit.png", MENU_START);
	int resumeButton = buttonHandler.AddButton(glm::vec2(0.0f, 0.35f), 0.35f, 0.35f, "Textures/Menu/resume.png", MENU_INGAME);
	int quitButtonInGame = buttonHandler.AddButton(glm::vec2(0.0f, -0.35f), 0.25f, 0.25f, "Textures/Menu/quit.png", MENU_INGAME);

	// Initiate timer
	double currentTime = 0;
	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double constLastTime = 0;
	int nrOfFrames = 0;

	//========================== Creating Landmarks =============================//

	ObjectHandler OH;
	CreateLandmarks(&OH, &maze);

	//=========================== Game Loop ====================================//

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
			display.SetTitle("FPS: " + to_string((nrOfFrames)));
			nrOfFrames = 0;
			lastTime += 1.0;
		}
		
		// ================== EVENTS ==================
		glfwPollEvents();
		HandleEvents(&player, &maze, &winSound, &deathSound, &minotaur, &display, &paused, &startMenu, &buttonHandler, &IH, &mazeGrid, enginePtr, &exit, &OH);

		if (!paused)
		{
			// Update movement
			IH.MouseControls(&display, &player, deltaTime);
			IH.KeyboardControls(&display, &player, deltaTime);

			// ================== UPDATE ==================

			// Update player
			player.Update(deltaTime);
			minotaur.Update(deltaTime, player.GetCamera()->GetCameraPosition());

			OH.UpdateAllObjects(deltaTime);
			maze.UpdateKeystones(deltaTime);
			lights.UpdateShadowTransform(0);

			// update sound engine with position and view direction
			soundEngine.Update(player.GetCamera()->GetCameraPosition(), player.GetCamera()->GetForwardVector());

		}
		else    // if game is paused
		{
			if (startMenu)
			{
			}
			else
			{
				// Update player
				player.UpdateOnlyTorch(deltaTime);
			}
		}

		// ================== DRAW ==================

		// Here the mazes is created and stored in a buffer with transform feedback
		MazeGenerationPass(&mazeGenerationShader, &maze, &player);
		
		// Here a cube map is calculated and stored in the shadowMap FBO
		ShadowPass(&shadowShader, &shadowAnimationShader, &OH, &lights, &shadowMap, &player, &minotaur, &maze, &exit);
		
		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, &geometryPass, &mazeGeometryPass, &animationPass, &player, &OH, &maze, &minotaur, &exit);
		
		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &finalFBO, &fullScreenQuad, &lightPass, &shadowMap, &lights, player.GetCamera());

		// Copy the depth from the gBuffer to the finalFBO
		finalFBO.CopyDepth(SCREEN_WIDTH, SCREEN_HEIGHT, gBuffer.GetFBO());

		// In this function, all particles will be drawn to the finalFBO
		ParticlePass(&finalFBO, player.GetTorch()->GetParticle(), player.GetCamera(), &particleShader);

		// Render everything
		FinalPass(&finalFBO, &finalShader, &fullScreenQuad);


		if (paused)
		{
			if (startMenu)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Button2DPass(&button2DShader, &buttonHandler, MENU_START);
			}
			else
			{
				Button2DPass(&button2DShader, &buttonHandler, MENU_INGAME);
			}
		}
		else
		{
			// Draw UI on top of everything else
			CoinUIPass(&coinUIShader, &coinInterfaceQuad, &player);
		}


		// ================== POST DRAW ==================
		display.SwapBuffers(SCREEN_WIDTH, SCREEN_HEIGHT);

		constLastTime = currentTime;
	}

	glfwTerminate();
	return 0;
}
