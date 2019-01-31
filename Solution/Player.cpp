#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(double dt, Camera camera)
{
	// Set player position to the cameras position
	transform.SetPos(camera.getCameraPosition());

	// Collision

}