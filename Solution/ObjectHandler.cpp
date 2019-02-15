#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	
}

ObjectHandler::~ObjectHandler()
{
	for (int i = 0; i < this->allObjects.size(); i++)
	{
		delete this->allObjects[i];
	}
}

int ObjectHandler::CreateObject(Mesh *mesh,  Texture *texture)
{
	Object* temp = new Object(mesh, texture);
	this->allObjects.push_back(temp);
	
	return this->allObjects.size()-1;
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

	Object* temp = new Object(mesh, texture);
	this->allObjects.push_back(temp);

	return this->allObjects.size() - 1;
}

void ObjectHandler::UpdateAllObjects(double dt)
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		allObjects[i]->Update(dt);
	}
}

unsigned int ObjectHandler::GetNrOfObjects()
{
	return this->allObjects.size();
}

Object* ObjectHandler::GetObject(int index)
{
	return this->allObjects[index];
}
