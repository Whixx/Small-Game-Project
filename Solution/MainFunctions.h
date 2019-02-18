#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

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
#include "MazeGeneratePNG.h"


// Inkludera inte allt ni ser
//#include <iostream>
//#include <glew\glew.h>
//#include "Display.h"
//#include "Mesh.h"
//#include "Texture.h"
//#include "Transform.h"
//#include "Camera.h"
//#include <ctime>
//#include "timer.h"
//#include <stdio.h>
//#include "Maze.h"
//#include <glm/gtc/type_ptr.hpp>

// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#undef main

#define PI 3.1415926535


// Shader initiation functions
void InitShadowShader(Shader *shader);
void InitGeometryPass(Shader *shader);
void InitLightPass(Shader *shader);
void InitParticleShader(Shader *shader);
void InitPointLightPass(Shader *shader);
void InitBlurShader(Shader *shader);
void InitFinalBloomShader(Shader *shader);
void InitFinalShader(Shader *shader);

// Shader pass functions
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Player *player, ObjectHandler *OH);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, GLuint *fullScreenQuad, GLuint *program, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, Model *renderModel);
void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader, float deltaTime, glm::vec3 position);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, GLuint *fullScreenTriangle);

// height and width must be odd numbers else the resulting maze will be off
void GenerateMazeBitmaps(int height, int width);

GLuint CreateScreenQuad();


#endif
