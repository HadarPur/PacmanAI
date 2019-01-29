#pragma once


const int MSIZE = 600;

const int SPACE = 1;
const int WALL = 2;

const int CELL_SIZE = 5;
const int SPACE_SIZE = CELL_SIZE * 5;

class Playground 
{

private:
	enum wallSide { Top, Bottom, Left, Right };    //the position of the wall
	int** maze;

public:
	void setupMaze(int***(maze));


	void thickenWallInMaze(wallSide side, int row, int col);

	void setupPerimeter();

	void setupCenterSquare();

	void setupCenterWalls();

	void setupTopSection();

	void setupBottomSection();

	void setupInitials();

};


