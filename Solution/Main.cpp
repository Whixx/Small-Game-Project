

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
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

	Display display;

	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(display.GetWindow(), InputHandler::Key_callback);

	EventHandler& EH = EventHandler::GetInstance();
	InputHandler IH;
	MaterialHandler& MH = MaterialHandler::GetInstance(); // Singleton

	// height and width must be odd numbers else the resulting maze will be off
	// inside the maze class the image will be made in to an even power of two number (ATM hardcoded 64) for use in shaders
	//GenerateMazeBitmaps(63, 63); // Creates maze.png + maze_d.png
	std::vector<std::vector<int>> mazeGrid = GenerateMazePNG(63, 63);

	// Sound engine that plays all the sounds, pass reference to classes that will use sound with enginePtr
	SoundEngine soundEngine;
	irrklang::ISoundEngine* enginePtr = soundEngine.GetEngine();

	Maze maze(enginePtr);

	//=========================== Creating Shaders ====================================//

	// MaxVertices supported by the hardware
	SetMaxPatchVertices();

	Shader mazeGenerationShader;
	mazeGenerationShader.CreateShader(".\\mazeGenerationShader.vs", GL_VERTEX_SHADER);
	mazeGenerationShader.CreateShader(".\\mazeGenerationShader.gs", GL_GEOMETRY_SHADER);
	mazeGenerationShader.CreateShader(".\\mazeGenerationShader.fs", GL_FRAGMENT_SHADER);
	
	Shader shadowShader;
	shadowShader.CreateShader(".\\shadowShader.vs", GL_VERTEX_SHADER);
	shadowShader.CreateShader(".\\shadowShader.gs", GL_GEOMETRY_SHADER);
	shadowShader.CreateShader(".\\shadowShader.fs", GL_FRAGMENT_SHADER);

	Shader geometryPass;
	geometryPass.CreateShader(".\\geometryPass.vs", GL_VERTEX_SHADER);
	geometryPass.CreateShader(".\\geometryPass.fs", GL_FRAGMENT_SHADER);

	Shader mazeGeometryPass;
	mazeGeometryPass.CreateShader(".\\mazeGeometryPass.vs", GL_VERTEX_SHADER);
	mazeGeometryPass.CreateShader(".\\mazeGeometryPass.cs", GL_TESS_CONTROL_SHADER);
	mazeGeometryPass.CreateShader(".\\mazeGeometryPass.es", GL_TESS_EVALUATION_SHADER);
	mazeGeometryPass.CreateShader(".\\mazeGeometryPass.fs", GL_FRAGMENT_SHADER);
	
	Shader lightPass;
	lightPass.CreateShader(".\\lightPass.vs", GL_VERTEX_SHADER);
	lightPass.CreateShader(".\\lightPass.fs", GL_FRAGMENT_SHADER);

	Shader particleShader;
	particleShader.CreateShader(".\\particleShader.vs", GL_VERTEX_SHADER);
	particleShader.CreateShader(".\\particleShader.fs", GL_FRAGMENT_SHADER);

	//Shader pointLightPass;
	//pointLightPass.CreateShader(".\\pointLightShader.vs", GL_VERTEX_SHADER);
	//pointLightPass.CreateShader(".\\pointLightShader.fs", GL_FRAGMENT_SHADER);

	Shader blurShader;
	blurShader.CreateShader(".\\blurShader.vs", GL_VERTEX_SHADER);
	blurShader.CreateShader(".\\blurShader.fs", GL_FRAGMENT_SHADER);

	Shader finalBloomShader;
	finalBloomShader.CreateShader(".\\finalBloomShader.vs", GL_VERTEX_SHADER);
	finalBloomShader.CreateShader(".\\finalBloomShader.fs", GL_FRAGMENT_SHADER);

	Shader finalShader;
	finalShader.CreateShader(".\\finalShader.vs", GL_VERTEX_SHADER);
	finalShader.CreateShader(".\\finalShader.fs", GL_FRAGMENT_SHADER);

	InitMazeGenerationShader(&mazeGenerationShader, &maze);
	InitShadowShader(&shadowShader);
	InitGeometryPass(&geometryPass);
	InitMazeGeometryPass(&mazeGeometryPass);
	InitLightPass(&lightPass);
	InitParticleShader(&particleShader);
	//InitPointLightPass(&pointLightPass);
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



	// Create Lights
	PointLightHandler lights;	// use .CreateLight()


	Sound winSound("Sounds/winSound.mp3", false, enginePtr);
	Sound deathSound("Sounds/death.mp3", false, enginePtr);
	Sound minotaurGrowlSound("Sounds/minotaurgrowl.wav", false, enginePtr);

	Minotaur minotaur(enginePtr, mazeGrid, &maze);
	float playerHeight = 1.8f;
	float torchSize = 0.02f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, &maze, enginePtr, &lights, torchSize, &minotaur);
	player.SetPlayerSpeed(2.0f);
	player.CenterPlayer(); //Space to return to origin

	minotaur.GetTransform().GetPos() = player.GetCamera()->GetCameraPosition();

	ObjectHandler OH;
	
	Model lightSphereModel("Models/Ball/ball.obj");
	GLuint screenQuad = CreateScreenQuad();

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

		glfwPollEvents();

		HandleEvents(&player, &maze, &winSound, &deathSound, &minotaurGrowlSound);

		// Update movement
		IH.MouseControls(&display, &player, deltaTime);
		IH.KeyboardControls(&display, &player, deltaTime);

		// ================== UPDATE ==================

		// Update player
		player.Update(deltaTime);
		minotaur.Update(player.GetCamera()->GetCameraPosition());

		OH.UpdateAllObjects(deltaTime);
		maze.UpdateKeystones(deltaTime);
		lights.UpdateShadowTransform(0);

		// update sound engine with position and view direction
		soundEngine.Update(player.GetCamera()->GetCameraPosition(), player.GetCamera()->GetForwardVector());


		// ================== DRAW ==================

		// Here the mazes is created and stored in a buffer with transform feedback
		MazeGenerationPass(&mazeGenerationShader, &maze, &player);
		
		// Here a cube map is calculated and stored in the shadowMap FBO
		ShadowPass(&shadowShader, &OH, &lights, &shadowMap, &player, &maze);
		
		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, &geometryPass, &mazeGeometryPass, &player, &OH, &maze, &minotaur);
		
		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &bloomBuffer, &screenQuad, &lightPass, &shadowMap, &lights, player.GetCamera());

		// Copy the depth from the gBuffer to the bloomBuffer
		bloomBuffer.CopyDepth(SCREEN_WIDTH, SCREEN_HEIGHT, gBuffer.GetFBO());

		// Draw lightSpheres
		//#ifdef DEBUG
		//	LightSpherePass(&pointLightPass, &bloomBuffer, &lights, player.GetCamera(), &lightSphereModel);
		//#endif
			
		// Blur the bright texture
		BlurPass(&blurShader, &bloomBuffer, &blurBuffers, &screenQuad);

		// Combine the bright texture and the scene and store the Result in FinalFBO.
		FinalBloomPass(&finalBloomShader, &finalFBO, &bloomBuffer, &blurBuffers, &screenQuad);

		// Copy the depth from the bloomBuffer to the finalFBO
		finalFBO.CopyDepth(SCREEN_WIDTH, SCREEN_HEIGHT, bloomBuffer.GetFBO());

		ParticlePass(&finalFBO, player.GetTorch()->GetParticle(), player.GetCamera(), &particleShader);

		// Render everything
		FinalPass(&finalFBO, &finalShader, &screenQuad);



		// ================== POST DRAW ==================
		display.SwapBuffers(SCREEN_WIDTH, SCREEN_HEIGHT);

		constLastTime = currentTime;
	}

	glfwTerminate();
	return 0;
}
