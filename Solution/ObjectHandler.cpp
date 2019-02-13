#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	this->numberOfObjects = 0;
}

ObjectHandler::~ObjectHandler()
{

}

int ObjectHandler::CreateObject(Mesh *mesh,  Texture *texture)
{
	this->allObjects[this->numberOfObjects] = Object(mesh, texture, this->numberOfObjects);

	return this->numberOfObjects++;
}

int ObjectHandler::CreateObject(const char* filePath, Mesh *mesh, Texture *texture)
{
	bool loaded = false;
	loaded = mesh->CreateMesh(filePath);

	if (loaded == true)
	{
#if DEBUG
		std::cout << "The object has been loaded correctly!" << std::endl;
#endif // DEBUG
	}
	else
	{
#if DEBUG
		std::cout << "Bad things happened during the attempt to load the object!" << std::endl;
#endif // DEBUG
	}

	this->allObjects[this->numberOfObjects] = Object(mesh, texture, this->numberOfObjects);

	return this->numberOfObjects++;
}

void ObjectHandler::UpdateAllObjects(double dt)
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		allObjects[i].Update(dt);
	}
}


unsigned int ObjectHandler::GetNrOfObjects()
{
	return this->numberOfObjects;
}

Object* ObjectHandler::GetObject(int index)
{
	return &this->allObjects[index];
}
