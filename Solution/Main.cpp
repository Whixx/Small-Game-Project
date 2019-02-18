
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include "MainFunctions.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	Display display;

	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(display.GetWindow(), InputHandler::Key_callback);

	InputHandler IH = InputHandler();


	//=========================== Creating Shaders ====================================//

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
	// height and width must be odd numbers else the resulting maze will be off
	// inside the maze class the image will be made in to an even power of two number (ATM hardcoded 64) for use in shaders
	GenerateMazeBitmaps(63, 63); // Creates maze.png + maze_d.png
	Maze maze = Maze("MazePNG/mazeBlackWhite.png");

	float playerHeight = 1.0f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, &maze);
	player.SetPlayerSpeed(5.0f);
	//player.CenterPlayer();

	ObjectHandler OH = ObjectHandler();

	//TODO: Byta ground.png till floor.png
	int floor = OH.CreateObject("Models/Floor/floor.obj");
	//int floor = OH.CreateObject("Models/Deer/12961_White-Tailed_Deer_v1_l2.obj");

	
	Model lightSphereModel("Models/Ball/ball.obj");
	GLuint screenQuad = CreateScreenQuad();
	
	//=================================================================================//

	
	// Create Lights
	PointLightHandler lights;
	PointLight torchLight;
	torchLight.GetColor() = glm::vec3(1.0f, 0.3f, 0.3f);
	lights.CreateLight(player.GetTorch()->GetPos(), torchLight.GetColor(), 2.0f);

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
		player.GetCamera()->UpdateViewMatrix();
		player.GetTorch()->Update(deltaTime);

		OH.UpdateAllObjects(deltaTime);

		lights.GetTransform(0)->GetPos() = glm::vec3(player.GetTorch()->GetPos().x, player.GetTorch()->GetPos().y + 1.5f, player.GetTorch()->GetPos().z);
		lights.UpdateShadowTransform(0);


		// ================== DRAW ==================

		// Here a cube map is calculated and stored in the shadowMap FBO
		ShadowPass(&shadowShader, &OH, &lights, &shadowMap, &player);

		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, &geometryPass, &player, &OH);

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
		ParticlePass(&finalFBO, &particle, player.GetCamera(), &particleShader, deltaTime, player.GetTorch()->GetPos());

		// Render everything
		FinalPass(&finalFBO, &finalShader, &screenQuad);



		// ================== POST DRAW ==================
		display.SwapBuffers(SCREEN_WIDTH, SCREEN_HEIGHT);

		constLastTime = currentTime;
	}
	glfwTerminate();
	return 0;
}
