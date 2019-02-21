#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H



// Finns en main funktion i GLEW, d�rmed m�ste vi undefinera den innan vi kan anv�nda v�ran main
#include <glew\glew.h>

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

// Shader pass functions
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Player *player, ObjectHandler *OH);
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
