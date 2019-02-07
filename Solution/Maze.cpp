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
void Maze::LoadMaze(const std::string & fileName)
{
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 3);

	if (imageData == NULL)
	{
		std::cerr << "Loading failed for texture: " << fileName << std::endl;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

unsigned char Maze::readPixel(unsigned int x, unsigned int y)
{
	return this->imageData[x*y*this->numComponents];
}

void Maze::Draw(unsigned int width, unsigned int height)
{
	glDrawArrays(GL_POINTS, 0, width * height);
	//glDrawElements(GL_POINTS, width* height, GL_UNSIGNED_INT, (void*)0);
}

void Maze::BindTexture(unsigned int textureUnit)
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
