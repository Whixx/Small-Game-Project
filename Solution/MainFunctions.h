#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H


#include "MazeGeneratePNG.h"

#include "Shader.h"
#include "ObjectHandler.h"
#include "PointLight.h"
#include "ShadowMap.h"
#include "Bloom.h"
#include "Blur.h"
#include "GBuffer.h"
#include "FinalFBO.h"
#include "Player.h"
#include "Maze.h"
#include "SoundHandler.h"
#include "Particle.h"
#include "InputHandler.h"
#include "MaterialHandler.h"




#define PI 3.1415926535

// Shader initiation functions
void InitWallShader(Shader *shader, Maze * maze);
void InitFloorShader(Shader * shader, Maze * maze);
void InitShadowShader(Shader *shader);
void InitGeometryPass(Shader *shader);
void InitLightPass(Shader *shader);
void InitParticleShader(Shader *shader);
void InitPointLightPass(Shader *shader);
void InitBlurShader(Shader *shader);
void InitFinalBloomShader(Shader *shader);
void InitFinalShader(Shader *shader);

void WallPass(Shader * wallShader, Maze * maze, Player * player);
void FloorPass(Shader * floorShader, Maze * maze, Player * player);

// Shader pass functions
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze* maze);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Player *player, ObjectHandler *OH, Maze* maze);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, GLuint *fullScreenQuad, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, Model *renderModel);
void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, GLuint *fullScreenTriangle);

// height and width must be odd numbers else the resulting maze will be off
void GenerateMazeBitmaps(int height, int width);

GLuint CreateScreenQuad();


#endif
