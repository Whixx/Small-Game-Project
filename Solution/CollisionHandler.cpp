#include "CollisionHandler.h"

CollisionHandler::CollisionHandler(Maze * maze)
{
	this->maze = maze;
}

CollisionHandler::~CollisionHandler()
{
}

bool CollisionHandler::CheckPlayerCollision(Player * player)
{
	return false;
}

bool CollisionHandler::CheckCoinCollision()
{
	return false;
}
