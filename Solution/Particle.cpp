#include "Particle.h"
#include <iostream>


Particle::Particle()
{
	// Preparing for the find function
	this->lastUsedParticle = 0;

	// Buffers that we're gonna fill up with the values of each particle. These will later be sent to the GPU.
	this->particlePosSizeBuffer = new GLfloat[maxParticles * 4];
	this->particleColorBuffer = new GLubyte[maxParticles * 4];

	// Fill upp the particleArray with init values
	for (int i = 0; i < maxParticles; i++)
	{
		particleArray[i].life = -1.0f;
		particleArray[i].cameradistance = -1.0f;
	}

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
	 -0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 -0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	};
	
	glGenBuffers(1, &this->billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &this->particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &this->particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

Particle::~Particle()
{
	delete[] particlePosSizeBuffer;
	delete[] particleColorBuffer;

	glDeleteBuffers(1, &billboard_vertex_buffer);
}

int Particle::findDeadParticle()
{
	// Double for loops to make this a bit more effective.
	for (int i = this->lastUsedParticle; i < maxParticles; i++) {
		if (particleArray[i].life < 0) {
			this->lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < this->lastUsedParticle; i++) {
		if (particleArray[i].life < 0) {
			this->lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are alive, override the first one
}

void Particle::sort()
{
	std::sort(&particleArray[0], &particleArray[maxParticles]);
}

void Particle::generateParticles(float deltaTime, glm::vec3 particlePos)
{
	int newParticlesToCreate = 2;

	for (int i = 0; i < newParticlesToCreate; i++)
	{
		// find an index to store the new particle in
		int index = this->findDeadParticle();

		// Set the start values of the new particle
		// Life (Length in seconds which describes how long the particle should be alive)
		this->particleArray[index].life = 2.0f;
		
		// Position
		this->particleArray[index].pos.x = particlePos.x;
		this->particleArray[index].pos.y = particlePos.y + 0.5f;
		this->particleArray[index].pos.z = particlePos.z;


		// Speed
		float speed = 0.80f;
		glm::vec3 mainDir = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 randomDir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 2000.0f,
			(rand() % 2000 - 1000.0f) / 2000.0f,
			(rand() % 2000 - 1000.0f) / 2000.0f
		);

		this->particleArray[index].speed = (mainDir + randomDir) * speed;

		// Color (white)
		this->particleArray[index].r = 255;
		this->particleArray[index].g = 255;
		this->particleArray[index].b = 255;
		this->particleArray[index].a = 255;

		// Size
		this->particleArray[index].size = 0.2f;
	}
}

void Particle::simulateParticles(glm::vec3 cameraPosition, float deltaTime)
{
	this->nrOfActiveParticles = 0;

	for (int i = 0; i < maxParticles; i++)
	{
		// Create temporary object of the particleStruct so we can work with it
		ParticleStruct& tempParticle = particleArray[i];

		// If the particle is still alive
		if (tempParticle.life > 0.0f)
		{
			// Decrease the life of each particle
			tempParticle.life -= deltaTime;

			// If the particle is still alive after decreased lifetime
			if (tempParticle.life > 0.0f)
			{
				// Update the attributes of the particle
				// Speed
				tempParticle.speed += deltaTime * 0.01f;

				// Position
				tempParticle.pos += tempParticle.speed * deltaTime;

				// Fading out
				if (tempParticle.a > 0.0f)
				{
					tempParticle.a -= deltaTime * 140;
					if (tempParticle.a < 0.0f)
					{
						tempParticle.a == 0.0f;
					}
				}

				// Changing color (white -> red)
				tempParticle.b -= deltaTime * 130.0;
				tempParticle.g -= deltaTime * 130.0;

				// CameraDistance
				tempParticle.cameradistance = glm::length(tempParticle.pos - cameraPosition);

				// Update the buffer that we're later gonna send to the GPU
				// Position and Size
				this->particlePosSizeBuffer[4 * this->nrOfActiveParticles + 0] = tempParticle.pos.x;
				this->particlePosSizeBuffer[4 * this->nrOfActiveParticles + 1] = tempParticle.pos.y;
				this->particlePosSizeBuffer[4 * this->nrOfActiveParticles + 2] = tempParticle.pos.z;
				this->particlePosSizeBuffer[4 * this->nrOfActiveParticles + 3] = tempParticle.size;

				// Color
				this->particleColorBuffer[4 * this->nrOfActiveParticles + 0] = tempParticle.r;
				this->particleColorBuffer[4 * this->nrOfActiveParticles + 1] = tempParticle.g;
				this->particleColorBuffer[4 * this->nrOfActiveParticles + 2] = tempParticle.b;
				this->particleColorBuffer[4 * this->nrOfActiveParticles + 3] = tempParticle.a;
			}
			else
			{
				// The particle is dead
				tempParticle.cameradistance = -1.0f;
			}
			this->nrOfActiveParticles++;
		}
	}
	this->sort();
}

// Update the buffers that OpenGL uses for rendering. Here we send our buffers to the GPU
void Particle::update()
{
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->nrOfActiveParticles * sizeof(GLfloat) * 4, this->particlePosSizeBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->nrOfActiveParticles * sizeof(GLubyte) * 4, this->particleColorBuffer);
}

// Bind every buffer (Vertexbuffer, PositionBuffer and ColorBuffer)
void Particle::bind()
{
	// Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboard_vertex_buffer);
	glVertexAttribPointer(
		0, // attribute. No particular reason for 0, but must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// Positions
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_position_buffer);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : x + y + z + size => 4
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_color_buffer);
	glVertexAttribPointer(
		2, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);
}

void Particle::draw()
{
	// Vertices, allways use the same vertices to make every particle (Hence the 0 parameter
	glVertexAttribDivisor(0, 0);

	// Positions, all the particles have 1 unique position each (Hence the 1 parameter)
	glVertexAttribDivisor(1, 1);

	// Color, all the particles have 1 unique Color each (Hence the 1 parameter)
	glVertexAttribDivisor(2, 1); 

	// Draw the particules!
	// glDrawArraysInstanced is like a forloop. It will draw every particle (the nrOfActiveParticles).
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->nrOfActiveParticles);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Particle::bindTexture()
{
	this->texture->Bind(0);
}

void Particle::setTexture(Texture* texture)
{
	this->texture = texture;
}
