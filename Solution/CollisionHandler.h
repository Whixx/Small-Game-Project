#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Player.h"
#include "Maze.h"

using namespace std;

class CollisionHandler
{
public:
	CollisionHandler(Maze *maze);
	~CollisionHandler();

	bool CheckPlayerCollision(Player *player);
	bool CheckCoinCollision();


private:
	Maze* maze;

};

#endif