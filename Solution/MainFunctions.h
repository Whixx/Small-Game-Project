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
void InitUserInterfaceShader(Shader *shader, Player * player);

// Shader pass functions
void MazeGenerationPass(Shader * mazeGenerationShader, Maze * maze, Player * player);
void ShadowPass(Shader *shadowShader, ObjectHandler *OH, PointLightHandler *PLH, ShadowMap *shadowFBO, Player *player, Maze* maze);
void DRGeometryPass(GBuffer *gBuffer, Shader *geometryPass, Shader *mazeGeometryPass, Player *player, ObjectHandler *OH, Maze* maze, Minotaur * minotaur);
void DRLightPass(GBuffer *gBuffer, BloomBuffer *bloomBuffer, GLuint *fullScreenQuad, Shader *geometryPass, ShadowMap *shadowBuffer, PointLightHandler *lights, Camera *camera);
void LightSpherePass(Shader *pointLightPass, BloomBuffer *bloomBuffer, PointLightHandler *lights, Camera *camera, Model *renderModel);
void BlurPass(Shader *blurShader, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void FinalBloomPass(Shader *finalBloomShader, FinalFBO * finalFBO, BloomBuffer *bloomBuffer, BlurBuffer *blurBuffers, GLuint *fullScreenTriangle);
void ParticlePass(FinalFBO * finalFBO, Particle * particle, Camera * camera, Shader * particleShader);
void FinalPass(FinalFBO * finalFBO, Shader * finalShader, GLuint *fullScreenTriangle);
void UserInterfacePass(Shader * userInterfaceShader, GLuint *quad, Texture * texture, Player * player);

// height and width must be odd numbers else the resulting maze will be off
// the function will return a grid with the maze
std::vector<std::vector<int>> GenerateMazePNG(int height, int width);

GLuint CreateScreenTriangle();
GLuint CreateSmallScreenQuad(glm::vec2 topLeftCorner, glm::vec2 topRightCorner, glm::vec2 bottomRightCorner, glm::vec2 bottomLeftCorner);
void SetMaxPatchVertices();

void HandleEvents(Player* player, Maze* maze, Sound* winSound, Sound* deathSound, Sound * minotaurGrowlSound, Minotaur * minotaur, bool* paused, Display* window);


#endif
