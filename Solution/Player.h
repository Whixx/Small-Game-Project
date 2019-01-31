#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

using namespace std;

class Player : public Object
{
public:
	Player(Mesh *mesh, Texture *texture);
	// Default constructor
	Player();
	virtual ~Player();

	virtual void Update();
	virtual void Draw();

private:

};

#endif //DISPLAY_H