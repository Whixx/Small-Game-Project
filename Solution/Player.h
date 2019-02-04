#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Maze.h"

using namespace std;

class Player
{
public:
	// Default constructor
	Player(Maze* maze);
	virtual ~Player();

	virtual void Update(double dt, Camera& camera);

private:
	Transform transform;

	Maze* maze;
};

#endif //DISPLAY_H