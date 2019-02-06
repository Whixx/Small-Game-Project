#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"

using namespace std;

class Player
{
public:
	// Default constructor
	Player();
	virtual ~Player();

	virtual void Update(double dt, Camera camera);

private:
	Transform transform;
};

#endif //DISPLAY_H