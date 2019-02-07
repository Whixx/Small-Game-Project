#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture(const std::string& textureName, const std::string& normalMapName)
{
	create(textureName, 0);
	//create(normalMapName, 1);
}

Texture::Texture(const Texture & other)
{
	this->m_texture = other.m_texture;
	//this->m_normalMap = other.m_normalMap;
}

void Texture::operator=(const Texture & other)
{
	this->m_texture = other.m_texture;
	//this->m_normalMap = other.m_normalMap;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
	glDeleteTextures(1, &m_normalMap);
}

void Texture::create(const std::string & fileName, bool nMap)
{
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 3);

	if (imageData == NULL)
	{
		std::cerr << "Loading failed for texture: " << fileName << std::endl;
	}

	if (nMap == false)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	else
	{
		glGenTextures(1, &m_normalMap);
		glBindTexture(GL_TEXTURE_2D, m_normalMap);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Skickar texturen till GPU'n
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::Bind(unsigned int unit)
{
	if (unit * 2 >= 0 && unit * 2 <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + unit * 2);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		//glActiveTexture(GL_TEXTURE0 + unit * 2 + 1);
		//glBindTexture(GL_TEXTURE_2D, m_normalMap);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}