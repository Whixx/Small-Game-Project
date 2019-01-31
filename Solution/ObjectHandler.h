#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <string>
#include "Object.h"
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler();

	int CreateObject(Mesh *mesh, Texture *texture);
	int CreateObject(const char* filePath, Mesh *mesh, Texture *texture);

	unsigned int getNrOfObjects();
	Object* getObject(int index);

private:
	unsigned int numberOfObjects;
	Object allObjects[256];

	
};

#endif