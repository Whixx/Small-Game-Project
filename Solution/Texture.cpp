#include "Texture.h"

Texture::Texture(const string & path, const string & type, bool gammaCorrection)
	: path(path), type(type), gammaCorrection(gammaCorrection), width(0), height(0), BPP(0), texture(0)
{
	// Load image
	this->LoadTexture();
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texture);
}

void Texture::Bind(unsigned int slot) const
{
	if (slot >= 0 && slot <= 31)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
	else
	{
		std::cout << "[ERROR] Texture could not be bound. Unit not in range[0-31]" << std::endl;
	}
}

void Texture::LoadTexture()
{
	//stbi_set_flip_vertically_on_load(1); // Flips the image upsidedown

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &BPP, 0);

	if (data)
	{
		GLenum dataformat;
		GLenum internalformat;
		if (BPP == 1)
		{
			dataformat = GL_RED;
			internalformat = GL_RED;
		}
		else if (BPP == 3)
		{
			dataformat = GL_RGB;
			internalformat = gammaCorrection ? GL_SRGB : GL_RGB;
		}
		else if (BPP == 4)
		{
			dataformat = GL_RGBA;
			internalformat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, dataformat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// How OpenGL Should apply the texture
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "[ERROR] Failed loading texture " << path << std::endl;
	}
}
