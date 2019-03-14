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
#include "Sound.h"
#include "Particle.h"
#include "InputHandler.h"
#include "MaterialHandler.h"
#include "Minotaur.h"
#include "EventHandler.h"
#include "ButtonHandler.h"

#include "Coin.h"




#define PI 3.1415926535

// Shader initiation functions
void InitMazeGenerationShader(Shader *shader, Maze * maze);
void InitShadowShader(Shader *shader);
void InitGeometryPass(Shader *shader);
void InitMazeGeometryPass(Shader *shader);
void InitLightPass(Shader *shader);
void InitParticleShader(Shader *shader);
void InitPointLightPass(Shader *shader);
void InitBlurShader(Shader *shader);
void InitFinalBloomShader(Shader *shader);
void InitFinalShader(Shader *shader);
void InitCoinUIShader(Shader *shader, Player * player);
void InitButton2DShader(Shader *shader);

// Shader pass functions
void MazeGenerationPass(Shader * mazeGenerationShader, Maze * maze, Player * player);
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze* maze);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Shader *mazeGeometryPass, Player *player, ObjectHandler *OH, Maze* maze, Minotaur * minotaur);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, ClipSpaceQuad * fullScreenQuad, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, Model *renderModel);
void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, ClipSpaceQuad * fullScreenQuad);
void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, ClipSpaceQuad * fullScreenQuad);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, ClipSpaceQuad * fullScreenQuad);
void CoinUIPass(Shader * coinUIShader, ClipSpaceQuad * coinInterfaceQuad, Texture * texture, Player * player);
void Button2DPass(Shader * button2DShader, ButtonHandler * buttonHandler, MENU_TYPE menuType);

// height and width must be odd numbers else the resulting maze will be off
// the function will return a grid with the maze
std::vector<std::vector<int>> GenerateMazePNG(int height, int width);

void SetMaxPatchVertices();

void HandleEvents(Player* player, Maze* maze, Sound* winSound, Sound* deathSound, Sound * minotaurGrowlSound, Minotaur * minotaur, bool* paused);


#endif
