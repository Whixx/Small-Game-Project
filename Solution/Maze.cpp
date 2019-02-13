#include "Maze.h"

Maze::Maze(string bitmapPath)
{
	this->imageData = nullptr;
	this->path = bitmapPath;
	this->width = 0;
	this->height = 0;
	this->numComponents = 0;
	this->texture = 0;

	LoadBMP(bitmapPath);
}

Maze::~Maze()
{
	stbi_image_free(imageData);
}

Maze::Maze(const Maze& other)
{
	this->path = other.path;
	this->width = other.width;
	this->height = other.height;
	this->numComponents = other.numComponents;

	this->LoadBMP(other.path);
}

Maze & Maze::operator=(const Maze & other)
{
	this->path = other.path;
	this->width = other.width;
	this->height = other.height;
	this->numComponents = other.numComponents;

	this->LoadBMP(other.path);

	return *this;
}

int Maze::GetMazeHeight()
{
	return this->height;
}

int Maze::GetMazeWidth()
{
	return this->width;
}

// Loading the .bmp file and returning the width,height and pointer to the first pixel in the file.
bool Maze::LoadBMP(const std::string & fileName)
{
	bool readingSuccess = true;

	this->imageData = stbi_load(fileName.c_str(), &this->width, &this->height, &this->numComponents, 4);

	if (this->imageData == NULL)
	{
		std::cerr << "Loading failed for bmp: " << fileName << std::endl;
		readingSuccess = false;
	}

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->imageData);

	return readingSuccess;
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

// Returns a vector with the rgb value of a pixel
glm::vec3 Maze::readPixel(unsigned int x, unsigned int y)
{
	unsigned int channelCount = 4;

	unsigned char* pixelOffset = this->imageData + (x + this->height * y) * channelCount;
	
	vector<unsigned char> pixel;
	for (int i = 0; i < 3; i++)
	{
		pixel.push_back(pixelOffset[i]);
	}

	return glm::vec3(pixel[0], pixel[1], pixel[2]);
}

void Maze::Bind(unsigned int textureUnit)
{
	if (textureUnit  >= 0 && textureUnit <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}
