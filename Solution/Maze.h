#ifndef MAZE_H
#define MAZE_H


class Maze
{
private:

public:
	bool IsWallAtWorld(float x, float y) { return true; }
	unsigned int GetMazeHeight() { return 64; }
	unsigned int GetMazeWidth() { return 64; }
};

#endif // !MAZE_H
