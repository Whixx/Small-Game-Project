#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm\glm.hpp>
#include <glew\glew.h>
// cstdlib for usage of rand()
#include <cstdlib>
// algorithm for usage of std::sort()
#include <algorithm>
#include "Texture.h"
#include "Camera.h"

#define maxParticles 500

struct ParticleStruct
{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	// We need to declare the operator< to be able to use std::sort.
	bool operator<(const ParticleStruct& that) const
	{
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particle
{
public:
	Particle(std::string path = "Textures/default/default_diffuse.png");
	~Particle();

	void GenerateParticles(float deltaTime, glm::vec3 particlePos);
	void SimulateParticles(glm::vec3 cameraPosition, float deltaTime);
	void Update(double deltaTime, Camera* camera, glm::vec3 position);
	void Bind();
	void Draw();
	Texture* GetTexture() const;
	void SetTexture(Texture* texture);
private:
	GLuint VA;
	GLuint billboardVertexBuffer;
	GLuint particlesPositionBuffer;
	GLuint particlesColorBuffer;

	ParticleStruct particleArray[maxParticles];
	GLfloat* particlePosSizeBuffer;
	GLubyte* particleColorBuffer;
	Texture* texture;

	// variables to find unused particles.. meaning particles that should be removed (life < 0)
	int lastUsedParticle;
	int findDeadParticle();

	int nrOfActiveParticles;

	void sort();
};

#endif