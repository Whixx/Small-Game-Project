#include "Maze.h"

Maze::Maze()
{
	this->imageData = nullptr;
	this->width = 0;
	this->height = 0;
	this->numComponents = 0;
	GLuint m_texture = 0;
}

Maze::~Maze()
{
	stbi_image_free(imageData);
}

// Loading the .bmp file and returning the width,height and pointer to the first pixel in the file.
bool Maze::loadBMP(const std::string & fileName)
{
	bool readingSuccess = true;

	this->imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Loading failed for bmp: " << fileName << std::endl;
		readingSuccess = false;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	return readingSuccess;
}

unsigned char Maze::readPixel(unsigned int x, unsigned int y)
{
	return this->imageData[x*y*this->numComponents];
}

void Maze::bind(unsigned int textureUnit)
{
	if (textureUnit  >= 0 && textureUnit <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}
