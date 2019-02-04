#include <iostream>
#include <glew\glew.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "ObjectHandler.h"
#include <ctime>
#include "timer.h"
#include "GBuffer.h"
#include "PointLight.h"
#include "Particle.h"
#include <stdio.h>
#include "Bloom.h"
#include "Blur.h"
#include "FinalFBO.h"
#include "ShadowMap.h"
#include "Player.h"

#include <glm/gtc/type_ptr.hpp>

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#undef main



#define PI 3.1415926535



int SCREENWIDTH = 800;
int SCREENHEIGHT = 600;

// Update functions
void updateAllObjects(double dt, ObjectHandler &OH);

// Shader pass functions
void shadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Camera *camera);
void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, GLuint cameraLocationGP, GLint texLoc, GLint normalTexLoc, int torch);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenQuad, GLuint *program, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, GLuint cameraLocationLP, Camera *camera);
void lightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, double counter);
void blurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void finalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle);
void particlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime);
void finalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh *fullScreenTriangle);

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera);
void prepareTexture(GLuint textureLoc, GLuint normalMapLoc);

void setStartPositions(ObjectHandler *OH);

void keyboardControls(Display *display, Camera *camera);
void mouseControls(Display *display, Camera *camera);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CRT_SECURE_NO_WARNINGS;

	Display display(SCREENWIDTH, SCREENHEIGHT);

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

	Camera camera(glm::vec3(-1, 1, 1), 70.0f, (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.01f, 1000.0f);
	

	//=========================== Creating Objects ====================================//
	Mesh groundMesh;
	Texture groundTexture("Textures/ground.png", "NormalMaps/ground_normal.png");
	Texture torchTexture("Textures/torch.png", "NormalMaps/torch_normal.png");

	ObjectHandler OH = ObjectHandler();

	Player player = Player();
	Mesh torchMesh;

	int ground = OH.CreateObject("ObjectFiles/ground.obj", &groundMesh, &groundTexture);
	int torch = OH.CreateObject("ObjectFiles/torch.obj", &torchMesh, &torchTexture);

	OH.getObject(torch)->GetPos() = glm::vec3(0.0f, -10.0f, 0.0f);
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
	double currentTime = glfwGetTime();
	double lastTime = 0;
	double deltaTime = 0;

	// Create Lights
	PointLightHandler lights;
	PointLight torchLight;
	float torchLightIntensity = 2.0f;
	torchLight.GetColor() = glm::vec3(1.0f, 0.5f, 0.5f) * torchLightIntensity;
	lights.createLight(OH.getObject(torch)->GetPos(), torchLight.GetColor());
	/*lights.createLight(glm::vec3(-7.0f, 7.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.createLight(glm::vec3(7.0f, 7.0f, 15.0f), glm::vec3(0.3f, 0.0f, 0.0f));*/

	lights.initiateLights(lightPass.getProgram());

	Particle particle;
	
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
		deltaTime = currentTime - lastTime;
		lastTime = glfwGetTime();


		// ================== UPDATE ==================
		player.Update(deltaTime, camera);
		updateAllObjects(deltaTime, OH);
		lights.updateShadowTransform(0);
			
		// Update the torch in front of the player'
		OH.getObject(torch)->GetPos() = camera.getCameraPosition()
			+ camera.getForwardVector() * 1.0f
			+ camera.getRightVector() * 0.5f
			+ camera.getUpVector() * -0.5f;
		//OH.getObject(torch)->GetRot() = camera.getCameraPosition();
		lights.getTransform(0)->GetPos() = glm::vec3(OH.getObject(torch)->GetPos().x, OH.getObject(torch)->GetPos().y + 0.8f, OH.getObject(torch)->GetPos().z);
		
		// Here a cube map is calculated and stored in the shadowMap FBO
		shadowPass(&shadowShader, &OH, &lights, &shadowMap, &camera);

		// ================== Geometry Pass - Deffered Rendering ==================
		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, counter, &geometryPass, &camera, &OH, cameraLocationGP, texLoc, normalTexLoc, torch);

		// ================== Light Pass - Deffered Rendering ==================
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &bloomBuffer, &fullScreenTriangle, lightPass.getProgram(), &lightPass, &shadowMap, &lights, cameraLocationLP, &camera);

		// Copy the depth from the gBuffer to the bloomBuffer
		bloomBuffer.copyDepth(SCREENWIDTH, SCREENHEIGHT, gBuffer.getFBO());

		// Draw lightSpheres
		#ifdef DEBUG
			lightSpherePass(&pointLightPass, &bloomBuffer, &lights, &camera, counter);
		#endif
			
		// Blur the bright texture
		blurPass(&blurShader, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Combine the bright texture and the scene and store the Result in FinalFBO.
		finalBloomPass(&finalBloomShader, &finalFBO, &bloomBuffer, &blurBuffers, &fullScreenTriangle);

		// Copy the depth from the bloomBuffer to the finalFBO
		finalFBO.copyDepth(SCREENWIDTH, SCREENHEIGHT, bloomBuffer.getFBO());

		// Draw particles to the FinalFBO
		particlePass(&finalFBO, &particle, &camera, &particleShader, deltaTime);

		// Render everything
		finalPass(&finalFBO, &finalShader, &fullScreenTriangle);

		//std::cout << "x: " << camera.getCameraPosition().x << " y: " << camera.getCameraPosition().y << " z: " << camera.getCameraPosition().z << std::endl;

		// Check for mouse/keyboard inputs and handle the camera movement
		mouseControls(&display, &camera);
		keyboardControls(&display, &camera);
		camera.updateViewMatrix();

		display.SwapBuffers(SCREENWIDTH, SCREENHEIGHT);

		display.SetTitle("FPS: " + to_string(1/deltaTime));

		counter += deltaTime * 0.5;
		currentTime = glfwGetTime();
	}
	glfwTerminate();
	return 0;
}

void updateAllObjects(double dt, ObjectHandler & OH)
{
	for (int i = 0; i < OH.getNrOfObjects(); i++)
	{
		OH.getObject(i)->Update(dt);
	}
}

void shadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Camera *camera)
{
	glEnable(GL_DEPTH_TEST);
	shadowShader->Bind();
	shadowFBO->bind();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	vector<glm::mat4> shadowTransforms;
	glm::vec3 lightPos;

	// For each light, we create a matrix and draws each light.
	for (unsigned int i = 0; i < PLH->getNrOfLights(); i++)
	{
		shadowTransforms = PLH->getShadowTransform(i);

		lightPos = PLH->getTransform(i)->GetPos();

		for (int j = 0; j < 6; ++j)
		{
			shadowShader->setMat4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);
		}
		shadowShader->sendFloat("farPlane", (float)FAR_PLANE);
		shadowShader->sendVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

		for (unsigned int j = 0; j < OH->getNrOfObjects(); j++)
		{
			shadowShader->Update(OH->getObject(j)->GetTransform(), *camera);
			OH->getObject(j)->bindTexture();
			OH->getObject(j)->Draw();
		}
	}

	glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);
	shadowShader->unBind();
	glDisable(GL_DEPTH_TEST);
}

void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, GLuint cameraLocationGP, GLint texLoc, GLint normalTexLoc, int torch)
{
	geometryPass->Bind();

	sendCameraLocationToGPU(cameraLocationGP, camera);
	prepareTexture(texLoc, normalTexLoc);

	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);

	

	// Update and Draw all objects
	for (unsigned int i = 0; i < OH->getNrOfObjects(); i++)
	{
		if (OH->getObject(i) == OH->getObject(torch))
		{
			geometryPass->sendFloat("illuminated", 3.0f);
		}
		else
		{
			geometryPass->sendFloat("illuminated", 1.0f);
		}
		geometryPass->Update(OH->getObject(i)->GetTransform(), *camera);
		OH->getObject(i)->bindTexture();
		OH->getObject(i)->Draw();
	}

	geometryPass->unBind();
}

void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, Mesh *fullScreenTriangle, GLuint *program, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, GLuint cameraLocationLP, Camera *camera)
{
	lightPass->Bind();

	lights->sendToShader();
	sendCameraLocationToGPU(cameraLocationLP, camera);

	// Bloom buffer, write finalColor and brightness.
	bloomBuffer->bindForWriting();
	bloomBuffer->bindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	lightPass->sendInt("gPosition", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	lightPass->sendInt("gDiffuse", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	lightPass->sendInt("gNormal", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	
	shadowBuffer->bindForReading(3);
	lightPass->sendInt("shadowMap", 3);
	lightPass->sendFloat("farPlane", (float)FAR_PLANE);

	lightPass->validateShaders();

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

	lightPass->unBind();
}

// This function draws particles to the screen.
void particlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime)
{
	finalFBO->bindForWriting();

	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	// PS = ParticleShader
	GLuint cameraRightWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraRightWorldPS");
	GLuint cameraUpWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraUpWorldPS");
	GLuint viewProjection = glGetUniformLocation(*particleShader->getProgram(), "viewProjectionMatrix");

	// Create new particles
	particle->generateParticles(deltaTime);

	// Simulate all the particles
	particle->simulateParticles(camera->getCameraPosition(), deltaTime);

	// Update the buffers holding the particles
	particle->update();

	// Bind the correct shader
	particleShader->Bind();

	// Send Uniforms
	glUniform3f(cameraRightWorldPS, camera->getRightVector().x, camera->getRightVector().y, camera->getRightVector().z);
	glUniform3f(cameraUpWorldPS, camera->getUpVector().x, camera->getUpVector().y, camera->getUpVector().z);
	glUniformMatrix4fv(viewProjection, 1, GL_FALSE, &camera->getViewProjection()[0][0]);

	// Bind the buffers holding the particles
	particle->bind();

	// Draw the particles, send a draw call to the GPU
	particle->draw();

	// Unbind the shader
	particleShader->unBind();
}

void lightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, double counter)
{
	bloomBuffer->bindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//lights->updateShadowTransform(0);

	pointLightPass->Bind();
	for (unsigned int i = 0; i < lights->getNrOfLights(); i++)
	{
		pointLightPass->Update(*lights->getTransform(i), *camera);
		lights->Draw(i);
	}
	pointLightPass->unBind();
	glDisable(GL_CULL_FACE);
}

void blurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	blurShader->Bind();
	int timesToBlur = 10;
	bool horizontal = true;
	bool firstBlur = true;

	//GLuint loc = glGetUniformLocation(*blurShader->getProgram(), "scene");

	for (int i = 0; i < timesToBlur; i++)
	{
		blurBuffers->bindForWriting(!horizontal);

		// First we read from the bloom fbo, to get a starting point of the blur. Then after that we will
		// blur the "blurred" texture over and over again, swapping between vertical and horizontal blurring.
		if (firstBlur == true)
		{
			bloomBuffer->bindForReadingBloomMap(0);
			blurShader->sendInt("horizontal", !horizontal);
			blurShader->sendInt("scene", 0);
		}
		else
		{
			blurBuffers->bindForReading(horizontal, 0);
			blurShader->sendInt("horizontal", !horizontal);

			blurShader->sendInt("scene", 0);
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

void finalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, Mesh *fullScreenTriangle)
{
	finalFBO->bindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	finalBloomShader->Bind();

	bloomBuffer->bindForReadingDiffuse();
	finalBloomShader->sendInt("scene", 0);

	blurBuffers->bindForReading(1, 1);
	finalBloomShader->sendInt("bright", 1);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);

}

void finalPass(FinalFBO * finalFBO, Shader * finalShader, Mesh * fullScreenTriangle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	finalFBO->bindForReading(0);

	finalShader->Bind();
	finalShader->sendInt("scene", 0);

	glDisable(GL_DEPTH_TEST);
	fullScreenTriangle->Draw();
	glEnable(GL_DEPTH_TEST);
}

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera)
{
	glUniform3f(cameraLocation, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
}

void prepareTexture(GLuint textureLoc, GLuint normalMapLoc)
{
	glUniform1i(textureLoc, 0);
	glUniform1i(normalMapLoc, 1);
}

void setStartPositions(ObjectHandler * OH)
{

}

void keyboardControls(Display *display, Camera *camera)
{
	int keyboardButton;
	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to set the initial camera position & viewDir.
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_W);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveForward();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_S);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveBackward();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_D);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveRight();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_A);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveLeft();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_R);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveUp();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_F);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveDown();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_SPACE);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->setCameraPosition(camera->getStartCameraPosition());
		camera->setForwardVector(camera->getStartForwardVector());
	}
}

void mouseControls(Display *display, Camera *camera)
{
	double mouseXpos;
	double mouseYpos;
	int mouseState;

	// Check for mouse inputs, used to drag the camera view around
	mouseState = glfwGetMouseButton(display->getWindow(), GLFW_MOUSE_BUTTON_LEFT);

	// Find mouseposition (This function updates the X,Y values of the mouse position.
	glfwGetCursorPos(display->getWindow(), &mouseXpos, &mouseYpos);
	if (mouseState == GLFW_PRESS)
	{
		//std::cout << "pressed" << std::endl;
		camera->mouseUpdate(glm::vec2(mouseXpos, mouseYpos));
	}
	else if (mouseState == GLFW_RELEASE)
	{
	
	}
}