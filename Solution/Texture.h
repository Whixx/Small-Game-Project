#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glew\glew.h>

class Texture
{
public:
	Texture(const std::string& textureName, const std::string& normalMapName);
	Texture(const Texture& other);	// Copy constructor
	void operator=(const Texture& other);
	Texture() {}

	void Bind(unsigned int unit);
	virtual ~Texture();
private:
	void create(const std::string& fileName, bool nMap);

	GLuint m_texture;
	GLuint m_normalMap;
};

#endif