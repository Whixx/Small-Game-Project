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

	// Currently the create object doesn't fill empty spots, only appends to the back of the vector
	int CreateObject(const char* filePath);

	void UpdateAllObjects(double dt);

	unsigned int GetNrOfObjects();
	Object* GetObject(int index);

private:
	std::vector<Object*> allObjects;

};

#endif