#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm\glm.hpp>
#include <glew\glew.h>
// cstdlib for usage of rand()
#include <cstdlib>
// algorithm for usage of std::sort()
#include <algorithm>
#include "Texture.h"

#define maxParticles 10000

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
	Particle();
	~Particle();

	void generateParticles(float deltaTime, glm::vec3 particlePos);
	void simulateParticles(glm::vec3 cameraPosition, float deltaTime);
	void update();
	void bind();
	void draw();
	void bindTexture();
	void setTexture(Texture* texture);
private:
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

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