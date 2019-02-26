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
	Material() {}
	//Material(const Material &other);
	//Material& operator=(const Material &other);
	~Material();

	void BindMaterial(Shader* shader);
	void BindMaterialArray(Shader* shader, unsigned int type);

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