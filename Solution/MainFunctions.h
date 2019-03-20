#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H


#include "MazeGeneratePNG.h"

#include "Shader.h"
#include "ObjectHandler.h"
#include "PointLight.h"
#include "ShadowMap.h"
//#include "Bloom.h"
//#include "Blur.h"
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
#include "Menu.h"
#include "AnimatedMesh.h"

#include "Coin.h"
#define PI 3.1415926535

// Shader initiation functions
void InitMazeGenerationShader(Shader *shader, Maze * maze);
void InitShadowShader(Shader *shader);
void InitShadowAnimationShader(Shader * shader);
void InitGeometryPass(Shader *shader);
void InitMazeGeometryPass(Shader *shader);
void InitAnimationPass(Shader *shader);
void InitLightPass(Shader *shader);
void InitParticleShader(Shader *shader);
void InitFinalShader(Shader *shader);
void InitCoinUIShader(Shader *shader, Player * player);
void InitButton2DShader(Shader *shader);

// Shader pass functions
void MazeGenerationPass(Shader * mazeGenerationShader, Maze * maze, Player * player);
void ShadowPass(Shader *shadowShader, Shader * shadowAnimation, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Minotaur* minotaur, Maze* maze, Exit * exit);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Shader *mazeGeometryPass, Shader *animationPass, Player *player, ObjectHandler *OH, Maze* maze, Minotaur * minotaur, Exit * exit);
void DRLightPass(GBuffer *gBuffer, FinalFBO* finalFBO, ClipSpaceQuad *fullScreenQuad, Shader *lightPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, ClipSpaceQuad * fullScreenQuad);
void CoinUIPass(Shader * coinUIShader, ClipSpaceQuad * coinInterfaceQuad, Player * player);
void Button2DPass(Shader * button2DShader, Menu * buttonHandler, MENU_TYPE menuType);

// height and width must be odd numbers else the resulting maze will be off
// the function will return a grid with the maze
std::vector<std::vector<int>> GenerateMazePNG(int height, int width);

void SetMaxPatchVertices();
void CreateLandmarks(ObjectHandler * OH, Maze * maze);
void ResetLandmarks(ObjectHandler * OH, Maze * maze);

void HandleEvents(Player* player, Maze * maze, Sound *winSound, Sound * deathSound, Sound * minotaurGrowlSound, Minotaur * minotaur, Display* window, bool* paused, bool* startMenu,
					Menu* buttonHandler, InputHandler* ih, std::vector<std::vector<int>>* mazeGrid, irrklang::ISoundEngine* enginePtr, Exit* exit, ObjectHandler* OH);

std::vector<std::vector<int>>* RegenerateMaze(std::vector<std::vector<int>>* mazeGrid, Maze* maze, irrklang::ISoundEngine* enginePtr);

void ResetGame(std::vector<std::vector<int>>* mazeGrid, Maze* maze, irrklang::ISoundEngine* enginePtr, Exit* exit, Player* player, Minotaur* minotaur, ObjectHandler* OH);

#endif
