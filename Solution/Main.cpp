#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include "MainFunctions.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	Display display;

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

	// False = Pos and Texcoord
	// True  = Pos and Color
	shadowShader.InitiateShaders(false);
	geometryPass.InitiateShaders(false);
	lightPass.InitiateShaders(false);
	particleShader.InitiateShaders(false);
	pointLightPass.InitiateShaders(true);
	blurShader.InitiateShaders(false);
	finalBloomShader.InitiateShaders(false);
	finalShader.InitiateShaders(false);
	
	shadowShader.ValidateShaders();
	geometryPass.ValidateShaders();
	// LightPass is validated before its drawcall (to fix a bug), so its not validated here
	// LightPass.ValidateShaders();
	particleShader.ValidateShaders();
	pointLightPass.ValidateShaders();
	blurShader.ValidateShaders();
	finalBloomShader.ValidateShaders();
	finalShader.ValidateShaders();

	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(display.GetWindow(), InputHandler::Key_callback);

	InputHandler IH = InputHandler();

	//=========================== Creating Objects ====================================//
	// height and width must be odd numbers else the resulting maze will be off
	// inside the maze class the image will be made in to an even power of two number (ATM hardcoded 64) for use in shaders
	GenerateMazeBitmaps(63, 63); // Creates maze.png + maze_d.png
	Maze maze = Maze("MazePNG/mazeBlackWhite.png");
	Mesh groundMesh;
	Mesh torchMesh;
	Texture torchTexture("Textures/torch.png", "NormalMaps/torch_normal.png");

	float playerHeight = 1.0f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, &torchMesh, &torchTexture, &maze);
	player.SetPlayerSpeed(5.0f);
	player.CenterPlayer();

	Texture groundTexture("Textures/ground.png", "NormalMaps/ground_normal.png");

	ObjectHandler OH = ObjectHandler();

	//TODO: Byta ground.png till floor.png
	int ground = OH.CreateObject("ObjectFiles/ground.obj", &groundMesh, &groundTexture);
	//int torch = OH.CreateObject("ObjectFiles/torch.obj", &torchMesh, &torchTexture);

	//OH.GetObject(torch)->GetScale() *= 0.1;
	
	//=================================================================================//

	ShadowMap shadowMap;
	shadowMap.Init();

	GBuffer gBuffer;
	gBuffer.Init(SCREENWIDTH, SCREENHEIGHT);

	BloomBuffer bloomBuffer;
	bloomBuffer.Init(SCREENWIDTH, SCREENHEIGHT);

	BlurBuffer blurBuffers;
	blurBuffers.Init(SCREENWIDTH, SCREENHEIGHT);

	FinalFBO finalFBO;
	finalFBO.Init(SCREENWIDTH, SCREENHEIGHT);

	


	// https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/
	Vertex fullScreenVerticesTriangle[] =
	{ 
		Vertex(glm::vec3(-1, 3, 0), glm::vec2(0.0,2.0)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(0.0,0.0)),
		Vertex(glm::vec3(3, -1, 0), glm::vec2(2.0,0.0)),
	};

	Mesh fullScreenTriangle(fullScreenVerticesTriangle, (sizeof(fullScreenVerticesTriangle) / sizeof(fullScreenVerticesTriangle[0])));

	// Initiate timer
	double currentTime = 0;
	double lastTime = glfwGetTime();
	double deltaTime = 0;
	double constLastTime = 0;
	int nrOfFrames = 0;

	// Create Lights
	PointLightHandler lights;
	PointLight torchLight;
	float torchLightIntensity = 2.0f;
	torchLight.GetColor() = glm::vec3(1.0f, 0.3f, 0.3f) * torchLightIntensity;
	//lights.CreateLight(OH.GetObject(torch)->GetPos(), torchLight.GetColor());
	lights.CreateLight(player.GetTorch().GetPos(), torchLight.GetColor());
	/*lights.CreateLight(glm::vec3(-7.0f, 7.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.CreateLight(glm::vec3(7.0f, 7.0f, 15.0f), glm::vec3(0.3f, 0.0f, 0.0f));*/
	
	lights.InitiateLights(lightPass.GetProgram());

	Particle particle;
	Texture particleTexture("Textures/particle.png", "NormalMaps/flat_normal.jpg");
	particle.SetTexture(&particleTexture);
	
	// Tell the shaders the name of the camera (GP = GeometeryPass, LP = LightPass)
	GLuint cameraLocationGP = glGetUniformLocation(*geometryPass.GetProgram(), "cameraPosGP");
	GLuint cameraLocationLP = glGetUniformLocation(*lightPass.GetProgram(), "cameraPosLP");

	GLint texLoc;
	GLint normalTexLoc;

	texLoc = glGetUniformLocation(*geometryPass.GetProgram(), "texture");
	normalTexLoc = glGetUniformLocation(*geometryPass.GetProgram(), "normalMap");

	GLuint viewProjection = glGetUniformLocation(*pointLightPass.GetProgram(), "viewProjectionMatrix");

	while (!display.IsWindowClosed())
	{
		// Calculate DeltaTime
		constLastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - constLastTime;



		// ================== EVENTS ==================

		// Update movement
		IH.MouseControls(&display, &player, deltaTime);
		IH.KeyboardControls(&display, &player, deltaTime);



		// ================== UPDATE ==================

		// Update player
		player.Update(deltaTime);
		player.GetCamera()->UpdateViewMatrix();
		player.GetTorch().Update(deltaTime);

		updateAllObjects(deltaTime, OH);
		lights.GetTransform(0)->GetPos() = glm::vec3(player.GetTorch().GetPos().x, player.GetTorch().GetPos().y + 1.5f, player.GetTorch().GetPos().z);
		lights.UpdateShadowTransform(0);

		// Measure fps
		nrOfFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			// If last print was more than 1 sec ago, print and reset timer
			display.SetTitle("FPS: " + to_string((int)((double)nrOfFrames)));
			nrOfFrames = 0;
			lastTime += 1.0;
		}

		// ================== DRAW ==================

		// Here a cube map is calculated and stored in the shadowMap FBO
		ShadowPass(&shadowShader, &OH, &lights, &shadowMap, &player);

		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, &geometryPass, &player, &OH, cameraLocationGP, texLoc, normalTexLoc);

		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &bloomBuffer, &fullScreenTriangle, lightPass.GetProgram(), &lightPass, &shadowMap, &lights, cameraLocationLP, player.GetCamera());

		// Copy the depth from the gBuffer to the bloomBuffer
		bloomBuffer.CopyDepth(SCREENWIDTH, SCREENHEIGHT, gBuffer.GetFBO());

		// Draw lightSpheres
		#ifdef DEBUG
			LightSpherePass(&pointLightPass, &bloomBuffer, &lights, player.GetCamera());
		#endif
			
		// Blur the bright texture
		BlurPass(&blurShader, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Combine the bright texture and the scene and store the Result in FinalFBO.
		FinalBloomPass(&finalBloomShader, &finalFBO, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Copy the depth from the bloomBuffer to the finalFBO
		finalFBO.CopyDepth(SCREENWIDTH, SCREENHEIGHT, bloomBuffer.GetFBO());

		// Draw particles to the FinalFBO
		ParticlePass(&finalFBO, &particle, player.GetCamera(), &particleShader, deltaTime, player.GetTorch().GetPos());

		// Render everything
		FinalPass(&finalFBO, &finalShader, &fullScreenTriangle);



		// ================== POST DRAW ==================
		display.SwapBuffers(SCREENWIDTH, SCREENHEIGHT);
	}
	glfwTerminate();
	return 0;
}
