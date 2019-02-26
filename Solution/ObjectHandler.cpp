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

int ObjectHandler::CreateObject(const char* filePath)
{
	Object* temp = new Object(filePath);
	this->allObjects.push_back(temp);

	return this->allObjects.size() - 1;
}

void ObjectHandler::UpdateAllObjects(double dt)
{
	for (int i = 0; i < this->allObjects.size(); i++)
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
