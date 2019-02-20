#include "Maze.h"

Maze::Maze()
{
	this->imageData = nullptr;
	this->path = "";
	this->width = 0;
	this->height = 0;
	this->numComponents = 0;
	this->texture = 0;

	this->wallTbo = 0;
	this->wallVbo = 0;
	this->wallVao = 0;

	this->floorTbo = 0;
	this->floorVbo = 0;
	this->floorVao = 0;
}

Maze::~Maze()
{
	stbi_image_free(imageData);

	glDeleteBuffers(1, &this->wallTbo);
	glDeleteBuffers(1, &this->wallVbo);
	glDeleteVertexArrays(1, &this->wallVao);
}
/*
Maze::Maze(const Maze& other)
{
	this->path = other.path;
	this->width = other.width;
	this->height = other.height;
	this->numComponents = other.numComponents;

	this->texture = 0;

	this->wallTbo = 0;
	this->wallVbo = 0;
	this->wallVao = 0;

	this->LoadMaze(other.path);
}

Maze & Maze::operator=(const Maze & other)
{
	this->path = other.path;
	this->width = other.width;
	this->height = other.height;
	this->numComponents = other.numComponents;

	this->texture = 0;

	this->wallTbo = 0;
	this->wallVbo = 0;
	this->wallVao = 0;

	this->LoadMaze(other.path);

	return *this;
}*/

int Maze::GetMazeHeight()
{
	return this->height;
}

int Maze::GetMazeWidth()
{
	return this->width;
}

bool Maze::IsWallAtWorld(float x, float y)
{
	bool isAWall = true;

	// NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

	glm::vec3 pixel = readPixel(x, y);

	if (pixel == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		isAWall = false;
	}

	return isAWall;
}

void Maze::BindTexture(unsigned int textureUnit)
{
	if (textureUnit >= 0 && textureUnit <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}

void Maze::InitiateBuffers()
{
	// Wall Buffers
	initiateWallBuffers();

	// Floor Buffers
	initiateFloorBuffers();
}
void Maze::LoadMaze(const std::string & fileName)
{
	this->path = fileName;

	this->imageData = stbi_load(fileName.c_str(), &this->width, &this->height, &this->numComponents, 3);

	if (this->imageData == NULL)
		std::cerr << "Loading failed for texture: " << fileName << std::endl;

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->imageData);
}
void Maze::DrawWallsToBuffer()
{
	// Skip the fragment shader
	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(this->wallVao);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->wallTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->wallVbo);

	// Perform transform feedback
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, this->width * this->height);
	glEndTransformFeedback();

	// Enable the fragment shader again
	glDisable(GL_RASTERIZER_DISCARD);

	// Something ...
	glFlush();

	glBindVertexArray(0);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void Maze::DrawFloorToBuffer()
{
	// Skip the fragment shader
	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(this->floorVao);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->floorTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->floorVbo);


	// Perform transform feedback
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, this->width * this->height);
	glEndTransformFeedback();

	// Enable the fragment shader again
	glDisable(GL_RASTERIZER_DISCARD);

	// Something ...
	glFlush();

	glBindVertexArray(0);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void Maze::DrawWalls()
{
	glBindVertexArray(this->wallVao);
	
	glDrawTransformFeedback(GL_TRIANGLES, this->wallTbo);

	// Memory barrier
	glBindVertexArray(0);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void Maze::DrawFloor()
{
	glBindVertexArray(this->floorVao);

	glDrawTransformFeedback(GL_TRIANGLES, this->floorTbo);

	// Memory barrier
	glBindVertexArray(0);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void Maze::initiateWallBuffers()
{
	// Three walls can be drawn with the same point
	GLint maxNrOfVertices = 18;

	// wallVao to draw points
	glGenVertexArrays(1, &this->wallVao);
	glBindVertexArray(this->wallVao);

	// create a buffer to hold the results of the transform feedback process.
	glGenBuffers(1, &this->wallVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->wallVbo);

	// allocate space (no data)
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * maxNrOfVertices * this->width * this->height +
		sizeof(glm::vec2) * maxNrOfVertices * this->width * this->height,
		NULL,							// no data passed
		GL_DYNAMIC_COPY);

	// Set the output Layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (const GLvoid*)(sizeof(glm::vec3)));

	// create and bind transform feedback object and buffer to write to.
	glGenTransformFeedbacks(1, &this->wallTbo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->wallTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->wallVbo);

	glBindVertexArray(0);
}

void Maze::initiateFloorBuffers()
{
	// Only one floor for each point
	GLint maxNrOfVertices = 6;

	// wallVao to draw points
	glGenVertexArrays(1, &this->floorVao);
	glBindVertexArray(this->floorVao);

	// create a buffer to hold the results of the transform feedback process.
	glGenBuffers(1, &this->floorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->floorVbo);

	// allocate space (no data)
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * maxNrOfVertices * this->width * this->height +
		sizeof(glm::vec2) * maxNrOfVertices * this->width * this->height,
		NULL,							// no data passed
		GL_DYNAMIC_COPY);

	// Set the output Layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (const GLvoid*)(sizeof(glm::vec3)));

	// create and bind transform feedback object and buffer to write to.
	glGenTransformFeedbacks(1, &this->floorTbo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->floorTbo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->floorVbo);

	glBindVertexArray(0);
}

// Returns a vector with the rgb value of a pixel
glm::vec3 Maze::readPixel(unsigned int x, unsigned int y)
{
	unsigned int channelCount = this->numComponents;

	unsigned char* pixelOffset = this->imageData + (x + this->height * y) * channelCount;

	vector<unsigned char> pixel;
	for (int i = 0; i < 3; i++)
	{
		pixel.push_back(pixelOffset[i]);
	}

	return glm::vec3(pixel[0], pixel[1], pixel[2]);
}

void Maze::printBufferId()
{
	std::cout << "wallTbo " << wallTbo << std::endl;
	std::cout << "wallVbo " << wallVbo << std::endl;
	std::cout << "wallVao " << wallVao << std::endl;
	std::cout << "floorTbo " << floorTbo << std::endl;
	std::cout << "floorVbo " << floorVbo << std::endl;
	std::cout << "floorVao " << floorVao << std::endl;
}
