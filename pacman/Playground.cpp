#include "Playground.h"


void Playground::thickenWallInMaze(wallSide side, int row = -1, int col = -1)
{

	for (int j = 0; j < CELL_SIZE; j++)
	{
		switch (side)
		{
		case Top:
			maze[(row == -1) ? j : (row - j)][col] = WALL;
			break;
		case Bottom:
			maze[(row == -1) ? j : (row + j)][col] = WALL;
			break;
		case Left:
			maze[row][(col == -1) ? j : (col + j)] = WALL;
			break;
		case Right:
			maze[row][(col == -1) ? j : (col - j)] = WALL;
			break;
		}
	}
}

void Playground::setupPerimeter()
{
	int i, k;

	//calclate the second y coordinate in maze of the horizontal line from bottom up
	int middleDoubleSpaceCells = (((MSIZE / 3) - SPACE_SIZE) / 2);
	middleDoubleSpaceCells -= middleDoubleSpaceCells % CELL_SIZE;    //need to be multiplication of CEEL_SIZE

	for (i = 0; i < MSIZE; i++)
	{
		if (i < MSIZE / 3 || i >(MSIZE / 3) * 2)    //only first third and last third are walls
		{
			thickenWallInMaze(Left, i); // left walls
			thickenWallInMaze(Right, i, MSIZE - CELL_SIZE); // right bound walls of the maze
		}
		else
		{    //draw the left and right middle sections first lines
			if ((i == MSIZE / 3) || (i == 2 * MSIZE / 3))
			{

				//for the lower edge , it's positive, for the top edge is negative
				middleDoubleSpaceCells = (i == MSIZE / 3) ? middleDoubleSpaceCells : (middleDoubleSpaceCells * (-1));

				for (k = 0; k <= MSIZE / 6; k++)
				{

					//top and bottom bounds of the middle section ,left and right sides
					thickenWallInMaze(Bottom, i, k); //bottom  left
					thickenWallInMaze(Bottom, i, MSIZE - k - 1); //bottom right
					thickenWallInMaze(Bottom, i + middleDoubleSpaceCells, k); //top left
					thickenWallInMaze(Bottom, i + middleDoubleSpaceCells, MSIZE - k - 1); //top right
				}
			}//set the bottom and top vertical walls in the middle section for left and right
			else if (((i >  MSIZE / 3) && (i < MSIZE / 3 + middleDoubleSpaceCells)) ||
				((i < 2 * MSIZE / 3) && (i > 2 * MSIZE / 3 - middleDoubleSpaceCells)))//set the top vertical walls in the middle section for left and right
			{
				thickenWallInMaze(Left, i, MSIZE / 6); // left
				thickenWallInMaze(Right, i, MSIZE - MSIZE / 6); // right
			}
		}
		thickenWallInMaze(Bottom, -1, i); //bottom wall
		thickenWallInMaze(Top, MSIZE - CELL_SIZE, i); // top bound walls of the maze
	}
}

void Playground::setupCenterSquare()
{
	int i, j;

	for (i = (MSIZE / 2) - SPACE_SIZE; i <= (MSIZE / 2) + SPACE_SIZE; i++)
	{
		//left wall
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE));
		//right wall
		thickenWallInMaze(Right, i, (MSIZE / 2) + (3 * SPACE_SIZE));

		if ((i == (MSIZE / 2) - SPACE_SIZE) || (i == (MSIZE / 2) + SPACE_SIZE))
		{
			for (j = (MSIZE / 2) - (3 * SPACE_SIZE); j < (MSIZE / 2) + (3 * SPACE_SIZE); j++)
			{
				if (i == (MSIZE / 2) + SPACE_SIZE)
				{
					if (j < (MSIZE / 2) - SPACE_SIZE || j >(MSIZE / 2) + SPACE_SIZE)
						thickenWallInMaze(Top, i, j);
				}
				else
					thickenWallInMaze(Bottom, i, j);
			}
		}
	}
}


void Playground::setupCenterWalls()
{
	int i, j;
	int middleDoubleSpaceCells = (((MSIZE / 3) - SPACE_SIZE) / 2);
	middleDoubleSpaceCells -= middleDoubleSpaceCells % CELL_SIZE;    //need to be multiplication of CEEL_SIZE

																	 // botom section
	for (i = MSIZE / 3; i < MSIZE / 3 + middleDoubleSpaceCells; i++)
	{
		//left wall
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE);
		thickenWallInMaze(Right, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE - SPACE_SIZE);

		//right wall
		thickenWallInMaze(Right, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE + SPACE_SIZE);
	}

	// top section
	for (i = 2 * MSIZE / 3 - middleDoubleSpaceCells; i < 2 * MSIZE / 3; i++)
	{
		//left wall
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE);
		thickenWallInMaze(Right, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE - SPACE_SIZE);

		//right wall
		thickenWallInMaze(Right, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE + SPACE_SIZE);
	}

	// left vertical lines
	for (j = (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE; j > (MSIZE / 2) - (3 * SPACE_SIZE) - 3 * SPACE_SIZE - CELL_SIZE; j--)
	{
		// top
		thickenWallInMaze(Top, 2 * MSIZE / 3 - middleDoubleSpaceCells, j);
		thickenWallInMaze(Bottom, 2 * MSIZE / 3, j);

		// bottom
		thickenWallInMaze(Top, MSIZE / 3, j);
		thickenWallInMaze(Bottom, MSIZE / 3 + middleDoubleSpaceCells, j);
	}

	// right vertical lines
	for (j = (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE; j < (MSIZE / 2) + (3 * SPACE_SIZE) + 3 * SPACE_SIZE + CELL_SIZE; j++)
	{
		// top
		thickenWallInMaze(Top, 2 * MSIZE / 3 - middleDoubleSpaceCells, j);	//bottom line
		thickenWallInMaze(Bottom, 2 * MSIZE / 3, j);	//top line

														// bottom
		thickenWallInMaze(Top, MSIZE / 3, j);	//bottom line
		thickenWallInMaze(Bottom, MSIZE / 3 + middleDoubleSpaceCells, j);	//top line
	}

}

void Playground::setupTopSection()
{
	int i, j;

	// bottom rectangle
	for (i = MSIZE - 7 * SPACE_SIZE + CELL_SIZE; i >= MSIZE - 8 * SPACE_SIZE + CELL_SIZE; i--)
	{
		thickenWallInMaze(Right, i, (MSIZE / 2) - SPACE_SIZE);
		thickenWallInMaze(Left, i, (MSIZE / 2) + SPACE_SIZE);

		for (j = MSIZE / 2 - SPACE_SIZE; j < MSIZE / 2 + SPACE_SIZE; j++)
		{
			if (i == MSIZE - 7 * SPACE_SIZE + CELL_SIZE || i == MSIZE - 8 * SPACE_SIZE + CELL_SIZE)
				thickenWallInMaze(Top, i, j);
		}
	}

	// top rectangle
	for (i = MSIZE - 2 * SPACE_SIZE; i >= MSIZE - 3 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, (MSIZE / 2) - SPACE_SIZE);
		thickenWallInMaze(Left, i, (MSIZE / 2) + SPACE_SIZE);

		for (j = MSIZE / 2 - SPACE_SIZE; j < MSIZE / 2 + SPACE_SIZE; j++)
		{
			if (i == MSIZE - 2 * SPACE_SIZE || i == MSIZE - 3 * SPACE_SIZE)
				thickenWallInMaze(Bottom, i, j);
		}
	}

	// left +
	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE);

		if ((i <= MSIZE - 2 * SPACE_SIZE && i > MSIZE - 3 * SPACE_SIZE - CELL_SIZE) || (i < MSIZE - 4 * SPACE_SIZE && i > MSIZE - 6 * SPACE_SIZE - CELL_SIZE))
			thickenWallInMaze(Right, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE - SPACE_SIZE);
	}

	for (j = (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE; j >= (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE - SPACE_SIZE; j--)
	{
		thickenWallInMaze(Bottom, MSIZE - 2 * SPACE_SIZE, j);
		thickenWallInMaze(Top, MSIZE - 6 * SPACE_SIZE, j);
	}

	for (j = 2 * SPACE_SIZE; j < 6 * SPACE_SIZE + CELL_SIZE; j++)
	{
		thickenWallInMaze(Bottom, MSIZE - 3 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, MSIZE - 4 * SPACE_SIZE, j);
	}

	for (i = MSIZE - 3 * SPACE_SIZE - CELL_SIZE; i > MSIZE - 4 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 2 * SPACE_SIZE);
	}

	// right +
	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE);

		if ((i <= MSIZE - 2 * SPACE_SIZE && i > MSIZE - 3 * SPACE_SIZE - CELL_SIZE) || (i < MSIZE - 4 * SPACE_SIZE && i > MSIZE - 6 * SPACE_SIZE - CELL_SIZE))
			thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE + SPACE_SIZE);
	}

	for (j = (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE; j <= (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE + SPACE_SIZE; j++)
	{
		thickenWallInMaze(Bottom, MSIZE - 2 * SPACE_SIZE, j);
		thickenWallInMaze(Top, MSIZE - 6 * SPACE_SIZE, j);
	}

	for (j = MSIZE - 2 * SPACE_SIZE; j > MSIZE - 6 * SPACE_SIZE - CELL_SIZE; j--)
	{
		thickenWallInMaze(Bottom, MSIZE - 3 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, MSIZE - 4 * SPACE_SIZE, j);
	}

	for (i = MSIZE - 3 * SPACE_SIZE - CELL_SIZE; i > MSIZE - 4 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Left, i, MSIZE - 2 * SPACE_SIZE);
	}

}

void Playground::setupBottomSection()
{
	int i, j;

	// right rectangle
	for (j = MSIZE - 2 * SPACE_SIZE; j > MSIZE / 2 - SPACE_SIZE; j--)
	{
		thickenWallInMaze(Bottom, 7 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, 6 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 7 * SPACE_SIZE - CELL_SIZE; i > 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, MSIZE - 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE / 2 - SPACE_SIZE);
	}


	// draw right +
	for (j = MSIZE - 2 * SPACE_SIZE; j > MSIZE - 7 * SPACE_SIZE; j--)
	{
		if ((j > MSIZE - 4 * SPACE_SIZE && j <= MSIZE - 2 * SPACE_SIZE) || ((j > MSIZE - 7 * SPACE_SIZE && j < MSIZE - 5 * SPACE_SIZE)))
		{
			thickenWallInMaze(Bottom, 4 * SPACE_SIZE - CELL_SIZE, j);
			thickenWallInMaze(Top, 3 * SPACE_SIZE + CELL_SIZE, j);
		}
	}

	for (i = 4 * SPACE_SIZE - CELL_SIZE; i > 3 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, MSIZE - 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE - 7 * SPACE_SIZE);
	}

	for (i = 4 * SPACE_SIZE - CELL_SIZE; i < 5 * SPACE_SIZE; i++)
	{
		thickenWallInMaze(Right, i, MSIZE - 4 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE - 5 * SPACE_SIZE);
	}

	for (i = 2 * SPACE_SIZE; i <= 3 * SPACE_SIZE + CELL_SIZE; i++)
	{
		thickenWallInMaze(Right, i, MSIZE - 4 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE - 5 * SPACE_SIZE);
	}

	for (j = MSIZE - 4 * SPACE_SIZE; j >= MSIZE - 5 * SPACE_SIZE; j--)
	{
		thickenWallInMaze(Bottom, 5 * SPACE_SIZE, j);
		thickenWallInMaze(Top, 2 * SPACE_SIZE, j);
	}

	// left rectangle
	for (j = 2 * SPACE_SIZE; j < MSIZE / 2 + SPACE_SIZE; j++)
	{
		thickenWallInMaze(Bottom, 4 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, 3 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 4 * SPACE_SIZE - CELL_SIZE; i > 3 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE / 2 + SPACE_SIZE);
	}

	// draw left +
	for (j = 2 * SPACE_SIZE; j < 7 * SPACE_SIZE; j++)
	{
		thickenWallInMaze(Bottom, 7 * SPACE_SIZE - CELL_SIZE, j);
		if ((j < 4 * SPACE_SIZE && j >= 2 * SPACE_SIZE) || (j > 5 * SPACE_SIZE && j < 7 * SPACE_SIZE))
			thickenWallInMaze(Top, 6 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 6 * SPACE_SIZE + CELL_SIZE; i > 5 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 4 * SPACE_SIZE);
		thickenWallInMaze(Left, i, 5 * SPACE_SIZE);
	}

	for (i = 7 * SPACE_SIZE - CELL_SIZE; i > 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, 7 * SPACE_SIZE);
	}

	for (j = 4 * SPACE_SIZE; j <= 5 * SPACE_SIZE; j++)
	{
		thickenWallInMaze(Top, 5 * SPACE_SIZE, j);
	}
}

void Playground::setupInitials()
{
	int i, j;

	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 8 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE));
		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE));

		for (j = MSIZE / 2 - 3 * SPACE_SIZE; j < MSIZE / 2 + 3 * SPACE_SIZE; j++)
		{
			if (i == MSIZE - 5 * SPACE_SIZE)
				thickenWallInMaze(Bottom, i, j);
		}
	}

	for (i = 2 * SPACE_SIZE; i <= 8 * SPACE_SIZE; i++)
	{
		if (i < 5 * SPACE_SIZE)
			thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) - CELL_SIZE);
		else if (i < 8 * SPACE_SIZE)
			thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE));

		for (j = MSIZE / 2 - 3 * SPACE_SIZE; j < MSIZE / 2 + 3 * SPACE_SIZE; j++)
		{
			if (i == 2 * SPACE_SIZE || i == 5 * SPACE_SIZE || i == 8 * SPACE_SIZE)
				thickenWallInMaze(Bottom, i, j);
		}
	}
}


void Playground::setupMaze(int*** (maze))
{
	//int i, j;
	//
	//this->maze = *maze;

	////clean up the maze
	//for (i = 0; i < MSIZE; i++/*= CELL_SIZE*/)
	//	for (j = 0; j < MSIZE; j++/*= CELL_SIZE*/)
	//		maze[i][j] = SPACE;

	//setupPerimeter();
	//setupCenterSquare();
	//setupInitials();
	//setupCenterWalls();
	//setupBottomSection();
	//setupTopSection();
}