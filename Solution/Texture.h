#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glew\glew.h>
#include "stb_image.h"
#include <iostream>

using namespace std;

class Texture
{
public:
	Texture(const string& path = "Textures/default_diffuse.png", const string& type = "TextureDiffuse", bool gammaCorrection = false);
	virtual ~Texture();

	void Bind(unsigned int slot = 0) const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline std::string GetPath() const { return path; }
	inline std::string GetType() const { return type; }

private:
	void LoadTexture();

	int width;
	int height;
	int BPP;
	bool gammaCorrection;
	string path;
	string type;

	GLuint texture;
};

#endif