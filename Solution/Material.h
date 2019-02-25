#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>

using namespace std;

class Material
{
public:
	Material(Texture* tex_diffuse, Texture* tex_ambient, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess, const char* name);
	~Material();

	void BindMaterial(Shader* shader);

	inline string GetMaterialName() const { return this->name; };

private:
	void SendMaterial(Shader* shader);
	void BindTextures(Shader* shader);

	string name;
	Texture* diffuse;
	Texture* ambient;
	Texture* specular;
	Texture* normal;
	Texture* height;
	float shininess;
};

#endif