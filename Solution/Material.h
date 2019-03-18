#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>

using namespace std;

class Material
{
public:
	Material(Texture* tex_diffuse, Texture* tex_emissive, Texture* tex_specular, Texture* tex_normal, Texture* tex_height, float shininess, glm::vec3 ambient, const char* name);
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
	Texture* emissive;
	Texture* specular;
	Texture* normal;
	Texture* height;
	float shininess;
	glm::vec3 ambient;
};

#endif