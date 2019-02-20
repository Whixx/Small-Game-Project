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
		this->particleArray[i].life = -1.0f;
		this->particleArray[i].cameradistance = -1.0f;
	}

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
	 -0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 -0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	};
	
	glGenBuffers(1, &this->billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &this->particlesPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particlesPositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &this->particlesColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particlesColorBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

Particle::~Particle()
{
	delete[] this->particlePosSizeBuffer;
	delete[] this->particleColorBuffer;

	glDeleteBuffers(1, &this->billboardVertexBuffer);
}

int Particle::findDeadParticle()
{
	// Double for loops to make this a bit more effective.
	for (int i = this->lastUsedParticle; i < maxParticles; i++)
	{
		if (this->particleArray[i].life < 0)
		{
			this->lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < this->lastUsedParticle; i++)
	{
		if (this->particleArray[i].life < 0)
		{
			this->lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are alive, override the first one
}

void Particle::sort()
{
	std::sort(&this->particleArray[0], &this->particleArray[maxParticles]);
}

void Particle::GenerateParticles(float deltaTime, glm::vec3 particlePos)
{
	int newParticlesToCreate = 5;
	
	for (int i = 0; i < newParticlesToCreate; i++)
	{
		// find an index to store the new particle in
		int index = this->findDeadParticle();

		// Set the start values of the new particle
		// Life (Length in seconds which describes how long the particle should be alive)
		this->particleArray[index].life = 1.0f;
		
		// Position
		float randomPosX = ((rand() % 100) - 50) / 1000.0f;
		float randomPosZ = ((rand() % 100) - 50) / 1000.0f;
		float randomHeight = (rand() % 20) / 100.0f;
		this->particleArray[index].pos.x = particlePos.x + randomPosX;
		this->particleArray[index].pos.y = particlePos.y + randomHeight;
		this->particleArray[index].pos.z = particlePos.z + randomPosZ;

		// Speed
		float speed = 0.50f;
		glm::vec3 mainDir = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 randomDir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 5000.0f,
			(rand() % 2000 - 1000.0f) / 5000.0f,
			(rand() % 2000 - 1000.0f) / 5000.0f
		);

		this->particleArray[index].speed = (mainDir + randomDir) * speed;

		// Color (white)
		this->particleArray[index].r = 255;
		this->particleArray[index].g = 255;
		this->particleArray[index].b = 255;
		this->particleArray[index].a = 150;

		// Size
		this->particleArray[index].size = 0.15f;
	}
}

void Particle::SimulateParticles(glm::vec3 cameraPosition, float deltaTime)
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

				if (tempParticle.life >= 0.8f)
				{
					// Changing color (white -> red)
					tempParticle.b = (tempParticle.life / 1.5) * 255.0f;
					tempParticle.g = (tempParticle.life / 1.5) * 255.0f;

					// Fading out
					tempParticle.a = (tempParticle.life / 1.5) * 150.0f;
				}
				else
				{
					// Set particle as smoke
					tempParticle.r = 50;
					tempParticle.g = 50;
					tempParticle.b = 50;

					// Fading out
					tempParticle.a = (tempParticle.life / 1.0) * 150.0f;
				}
				
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
void Particle::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->particlesPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->nrOfActiveParticles * sizeof(GLfloat) * 4, this->particlePosSizeBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, this->particlesColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->nrOfActiveParticles * sizeof(GLubyte) * 4, this->particleColorBuffer);
}

// Bind every buffer (Vertexbuffer, PositionBuffer and ColorBuffer)
void Particle::Bind()
{
	// Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->billboardVertexBuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, this->particlesPositionBuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, this->particlesColorBuffer);
	glVertexAttribPointer(
		2, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);
}

void Particle::Draw()
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

void Particle::BindTexture()
{
	this->texture->Bind(0);
}

void Particle::SetTexture(Texture* texture)
{
	this->texture = texture;
}
