#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include "MainFunctions.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	// height and width must be odd numbers else the resulting maze will be off
	// inside the maze class the image will be made in to an even power of two number (ATM hardcoded 64) for use in shaders
	generateMazeBitmaps(63, 63);

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
	shadowShader.initiateShaders(false);
	geometryPass.initiateShaders(false);
	lightPass.initiateShaders(false);
	particleShader.initiateShaders(false);
	pointLightPass.initiateShaders(true);
	blurShader.initiateShaders(false);
	finalBloomShader.initiateShaders(false);
	finalShader.initiateShaders(false);
	
	shadowShader.validateShaders();
	geometryPass.validateShaders();
	// LightPass is validated before its drawcall (to fix a bug), so its not validated here
	// LightPass.validateShaders();
	particleShader.validateShaders();
	pointLightPass.validateShaders();
	blurShader.validateShaders();
	finalBloomShader.validateShaders();
	finalShader.validateShaders();

	glm::vec3 playerVector = glm::vec3(0.3f, 0, 1);
	float playerHeight = 1.0f;
	Player player = Player(playerHeight, 70.0f, 0.1f, 100.0f, playerVector);
	player.SetPlayerSpeed(5.0f);

	glfwSetInputMode(display.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(display.GetWindow(), InputHandler::key_callback);
	//=========================== Creating Objects ====================================//
	Mesh groundMesh;
	Texture groundTexture("Textures/ground.png", "NormalMaps/ground_normal.png");
	Texture torchTexture("Textures/torch.png", "NormalMaps/torch_normal.png");

	InputHandler IH = InputHandler();
	// The maze
	Maze maze = Maze("Bitmap/kollision_test.bmp");

	ObjectHandler OH = ObjectHandler();

	Mesh torchMesh;

	//TODO: Byta ground.png till floor.png
	int ground = OH.CreateObject("ObjectFiles/ground.obj", &groundMesh, &groundTexture);
	int torch = OH.CreateObject("ObjectFiles/torch.obj", &torchMesh, &torchTexture);

	OH.getObject(torch)->GetScale() *= 0.1;
	
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

	double counter = 0.0;

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
	lights.createLight(OH.getObject(torch)->GetPos(), torchLight.GetColor());
	/*lights.createLight(glm::vec3(-7.0f, 7.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.createLight(glm::vec3(7.0f, 7.0f, 15.0f), glm::vec3(0.3f, 0.0f, 0.0f));*/

	lights.initiateLights(lightPass.getProgram());

	Particle particle;
	Texture particleTexture("Textures/particle.png", "NormalMaps/flat_normal.jpg");
	particle.setTexture(&particleTexture);
	
	// Tell the shaders the name of the camera (GP = GeometeryPass, LP = LightPass)
	GLuint cameraLocationGP = glGetUniformLocation(*geometryPass.getProgram(), "cameraPosGP");
	GLuint cameraLocationLP = glGetUniformLocation(*lightPass.getProgram(), "cameraPosLP");

	GLint texLoc;
	GLint normalTexLoc;

	texLoc = glGetUniformLocation(*geometryPass.getProgram(), "texture");
	normalTexLoc = glGetUniformLocation(*geometryPass.getProgram(), "normalMap");

	GLuint viewProjection = glGetUniformLocation(*pointLightPass.getProgram(), "viewProjectionMatrix");

	while(!display.IsWindowClosed())
	{
		// ================== UPDATE ==================
		player.Update(deltaTime);
		player.GetCamera()->updateViewMatrix();
		updateAllObjects(deltaTime, OH);
		lights.updateShadowTransform(0);
				
		// Update the time
		constLastTime = currentTime;
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

		counter += deltaTime * 0.5;

		// Update movement
		IH.mouseControls(&display, &player, deltaTime);
		IH.keyboardControls(&display, &player, deltaTime);
	
		// Update the torch in front of the player'
		OH.getObject(torch)->GetPos() = player.GetCamera()->getCameraPosition()
			+ player.GetWalkingVector() * 0.8f
			+ player.GetCamera()->getRotateAround() * 0.4f
			+ player.GetCamera()->getUpVector() * -0.5f;
		lights.getTransform(0)->GetPos() = glm::vec3(OH.getObject(torch)->GetPos().x, OH.getObject(torch)->GetPos().y + 1.5f, OH.getObject(torch)->GetPos().z);
		
		// ================== MAIN FUNCTIONS ==================

		// Here a cube map is calculated and stored in the shadowMap FBO
		shadowPass(&shadowShader, &OH, &lights, &shadowMap, player.GetCamera());

		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, counter, &geometryPass, player.GetCamera(), &OH, cameraLocationGP, texLoc, normalTexLoc, torch);

		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &bloomBuffer, &fullScreenTriangle, lightPass.getProgram(), &lightPass, &shadowMap, &lights, cameraLocationLP, player.GetCamera());

		// Copy the depth from the gBuffer to the bloomBuffer
		bloomBuffer.copyDepth(SCREENWIDTH, SCREENHEIGHT, gBuffer.getFBO());

		// Draw lightSpheres
		#ifdef DEBUG
			lightSpherePass(&pointLightPass, &bloomBuffer, &lights, player.GetCamera(), counter);
		#endif
			
		// Blur the bright texture
		blurPass(&blurShader, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Combine the bright texture and the scene and store the Result in FinalFBO.
		finalBloomPass(&finalBloomShader, &finalFBO, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Copy the depth from the bloomBuffer to the finalFBO
		finalFBO.copyDepth(SCREENWIDTH, SCREENHEIGHT, bloomBuffer.getFBO());

		// Draw particles to the FinalFBO
		particlePass(&finalFBO, &particle, player.GetCamera(), &particleShader, deltaTime, OH.getObject(torch)->GetPos());

		// Render everything
		finalPass(&finalFBO, &finalShader, &fullScreenTriangle);

		// ================== SWAP BUFFERS ==================
		display.SwapBuffers(SCREENWIDTH, SCREENHEIGHT);
	}
	glfwTerminate();
	return 0;
}
