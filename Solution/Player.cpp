#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(Maze* maze)
{
	this->maze = maze;
}

Player::~Player()
{
}

void Player::Update(double dt, Camera & camera)
{
	// Test variables
	double x = 0.0;
	double y = 0.0;


	// Move player
	x = 3.0;
	y = 0.0;


	// Check collision
	if (maze->IsWallAtWorld(x, y))
	{
		// Don't move
		cout << "VÄGGGGG" << endl; // TODO: Remove after testing
	}

}