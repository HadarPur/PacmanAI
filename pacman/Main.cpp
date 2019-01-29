/**************************************************************************************************

WHEN THAT CODE WAS WRITTEN ONLY GOD, MY PARTNER AND ME KNEW HOW IT WORKS...

NOW...ONLY GOD KNOWS, AND EVEN THAT IS DISPUTABLE.

GOOD LUCK!!!

**************************************************************************************************/

#pragma warning(disable:4996)

#include "GLUT.H"
#include <math.h>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <set>


#include "Point2D.h"
#include "pacmanPawn.h"
#include "Monster.h"
#include "ComparePointsByDist.h"


using namespace std;

const int MSIZE = 600;
const int W = MSIZE; // window width
const int H = MSIZE; // window height

const int SPACE = 1;
const int WALL = 2;
const int COIN = 3;
const int UNREACHABLE = 4;
const int CENTER = 5;
const int TARGET_COIN = 6;
const int PACMAN_VISITED = 7;


const int MONSTER1_VISITED = 1;
const int MONSTER2_VISITED = 2;
const int MONSTER3_VISITED = 4;

unsigned char picture[MSIZE][MSIZE][3];

/*******************************************************************/
/*        ALWAYS WORK WITH MULTIPLICATION OF CELL_SIZE IN THE MAZE
OTHERWISE IT WON'T DRAW IT!!!!!!!!!!

And The origin : x -> from left to right,
y -> from bottom to top.
*/
/*******************************************************************/


const double SQSIZE = 2.0 / MSIZE;
int maze[MSIZE][MSIZE];
const int CELL_SIZE = 5;
const int SPACE_SIZE = CELL_SIZE * 5;
const double step = 0.1;
int stepCounter = 0;

enum wallSide { Top, Bottom, Left, Right };    //the position of the wall

/****************************************************************/
/*								COINS							*/
/****************************************************************/

vector<Point2D*> gray_coins;    //for bfs ditrebution of the coins
vector<Point2D*> coins_locations;

bool coinsBfs = false;
Point2D* parent_forCoins[MSIZE][MSIZE];
int coinCounter = 0;
const int COINS_STEP = 5 * CELL_SIZE;
int startPointForCoins_x = 3 * CELL_SIZE;
int startPointForCoins_y = 3 * CELL_SIZE;

const int MAX_COINS = pow((MSIZE - 2 * startPointForCoins_x) / COINS_STEP, 2);

/****************************************************************/
/*								PACMAN							*/
/****************************************************************/

Point2D* pacmanStartPoint = new Point2D(startPointForCoins_x, startPointForCoins_y);
PacmanPawn pacman(pacmanStartPoint, 2 * CELL_SIZE);
Point2D* nearestCoinToPacman;
vector<Point2D*> nearestCoinsVector;
Point2D* parent_pacman[MSIZE][MSIZE];
int pacmanMaze[MSIZE][MSIZE];


bool startPacmanCoinSearch = false;
priority_queue <Point2D*, vector<Point2D*>, ComparePointsByDist> pacmanRoute;
Point2D* lastPointForPath;

vector<Point2D*> pacmanPath;
bool checkVertical = true;
bool checkHorizontal = true;
int rowToCheck;
int colToCheck;

bool isPacmanCaught1 = false;
bool isPacmanCaught2 = false;
bool isPacmanCaught3 = false;
int pacman_Astar_Counter = 0;



/****************************************************************/
/*								MONSTERS						*/
/****************************************************************/
Point2D* monstersEscapePointFromCenter = new Point2D(300, 350);

Point2D* monster1_startPoint = new Point2D(300, 300);
Monster monster1(vector<double>({ 0, 1, 1 }), monster1_startPoint , 2 * CELL_SIZE);
Point2D* parent_monster1[MSIZE][MSIZE];
priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist> monster1_grayPQ;
vector<Point2D*> monster1_path;

Point2D* monster2_startPoint = new Point2D(350, 300);
Monster monster2(vector<double>({ 0, 1, 0 }), monster2_startPoint, 2 * CELL_SIZE);
Point2D* parent_monster2[MSIZE][MSIZE];
priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist> monster2_grayPQ;
vector<Point2D*> monster2_path;

Point2D* monster3_startPoint = new Point2D(250, 300);
Monster monster3(vector<double>({ 1, 0, 0 }), monster3_startPoint, 2 * CELL_SIZE);
Point2D* parent_monster3[MSIZE][MSIZE];
priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist> monster3_grayPQ;
vector<Point2D*> monster3_path;


int monstersMaze[MSIZE][MSIZE];
int monster_Astar_Counter = 0;
int monster_Astar_Counter_Arr[3];

bool gameOver = false;
bool pacmanWon = false;


/****************************************************************/
/*						GAME BOARD SETUP						*/
/****************************************************************/

void thickenWallInMaze(wallSide side, int row = -1, int col = -1)
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

void setUnreachableZone(int bottomRow, int topRow, int leftCol, int rightCol, int unreachableParam = UNREACHABLE)
{
	for (int i = bottomRow; i < topRow; i++)
		for (int j = leftCol; j < rightCol; j++)
			maze[i][j] = unreachableParam;

}

void setupPerimeter()
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
					if (middleDoubleSpaceCells < 0)
					{
						//top bounds of the middle section ,left and right sides
						thickenWallInMaze(Bottom, i, k); //top left
						thickenWallInMaze(Bottom, i, MSIZE - k - 1); //top right
						thickenWallInMaze(Bottom, i + middleDoubleSpaceCells, k); //bottom left
						thickenWallInMaze(Bottom, i + middleDoubleSpaceCells, MSIZE - k - 1); //bottom right

					}
					else
					{
						//bottom bounds of the middle section ,left and right sides
						thickenWallInMaze(Top, i, k); //bottom  left
						thickenWallInMaze(Top, i, MSIZE - k - 1); //bottom right
						thickenWallInMaze(Top, i + middleDoubleSpaceCells, k); //top left
						thickenWallInMaze(Top, i + middleDoubleSpaceCells, MSIZE - k - 1); //top right
					}
				}
				if (i == MSIZE / 3)
				{
					setUnreachableZone(i + 1, i + middleDoubleSpaceCells, 0, MSIZE / 6);
					setUnreachableZone(i + 1, i + middleDoubleSpaceCells, MSIZE - (MSIZE / 6), MSIZE);
				}
				else
				{
					setUnreachableZone(i + middleDoubleSpaceCells + 1, i, 0, MSIZE / 6);
					setUnreachableZone(i + middleDoubleSpaceCells + 1, i, MSIZE - (MSIZE / 6) + 1, MSIZE);
				}


			}//set the bottom and top vertical walls in the middle section for left and right
			else if ((i > MSIZE / 3) && (i < MSIZE / 3 + middleDoubleSpaceCells + 2 * CELL_SIZE))
			{
				thickenWallInMaze(Left, i - CELL_SIZE, MSIZE / 6); // left
				thickenWallInMaze(Right, i - CELL_SIZE, MSIZE - MSIZE / 6); // right
			}

			else if ((i < 2 * MSIZE / 3) && (i > 2 * MSIZE / 3 - middleDoubleSpaceCells - 2 * CELL_SIZE))//set the top vertical walls in the middle section for left and right
			{
				thickenWallInMaze(Left, i + CELL_SIZE, MSIZE / 6); // left
				thickenWallInMaze(Right, i + CELL_SIZE, MSIZE - MSIZE / 6); // right
			}
		}
		thickenWallInMaze(Bottom, -1, i); //bottom wall
		thickenWallInMaze(Top, MSIZE - CELL_SIZE, i); // top bound walls of the maze
	}
}

void setupCenterSquare()
{
	int i, j;

	for (i = (MSIZE / 2) - SPACE_SIZE; i < (MSIZE / 2) + SPACE_SIZE + CELL_SIZE; i++)
	{
		//left wall
		thickenWallInMaze(Right, i, (MSIZE / 2) - (3 * SPACE_SIZE));
		//right wall
		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE));

		if ((i == (MSIZE / 2) - SPACE_SIZE) || (i == (MSIZE / 2) + SPACE_SIZE))
		{
			for (j = (MSIZE / 2) - (3 * SPACE_SIZE); j < (MSIZE / 2) + (3 * SPACE_SIZE); j++)
			{
				if (i == (MSIZE / 2) + SPACE_SIZE)
				{
					if (j < (MSIZE / 2) - SPACE_SIZE || j >(MSIZE / 2) + SPACE_SIZE)
						thickenWallInMaze(Bottom, i, j);
				}
				else
					thickenWallInMaze(Top, i, j);
			}
		}
	}

	setUnreachableZone((MSIZE / 2) - SPACE_SIZE + 1, (MSIZE / 2) + SPACE_SIZE,
		(MSIZE / 2) - (3 * SPACE_SIZE) + 1, (MSIZE / 2) + (3 * SPACE_SIZE), CENTER);
}


void setupCenterWalls()
{
	int i, j;
	int middleDoubleSpaceCells = (((MSIZE / 3) - SPACE_SIZE) / 2);
	middleDoubleSpaceCells -= middleDoubleSpaceCells % CELL_SIZE;    //need to be multiplication of CEEL_SIZE

																	 // botom section
	for (i = MSIZE / 3; i < MSIZE / 3 + middleDoubleSpaceCells + CELL_SIZE; i++)
	{
		//left wall
		thickenWallInMaze(Left, i, (MSIZE / 2) - (5 * SPACE_SIZE));
		thickenWallInMaze(Right, i, (MSIZE / 2) - (6 * SPACE_SIZE));

		//right wall
		thickenWallInMaze(Right, i, (MSIZE / 2) + (5 * SPACE_SIZE));
		thickenWallInMaze(Left, i, (MSIZE / 2) + (6 * SPACE_SIZE));
	}

	//bottom left
	setUnreachableZone(MSIZE / 3 + 1, MSIZE / 3 + middleDoubleSpaceCells,
		(MSIZE / 2) - (6 * SPACE_SIZE) + 1, (MSIZE / 2) - (5 * SPACE_SIZE));
	//bottom right
	setUnreachableZone(MSIZE / 3 + 1, MSIZE / 3 + middleDoubleSpaceCells,
		(MSIZE / 2) + (5 * SPACE_SIZE) + 1, (MSIZE / 2) + (6 * SPACE_SIZE));


	// top section
	for (i = 2 * MSIZE / 3 - middleDoubleSpaceCells; i < 2 * MSIZE / 3 + CELL_SIZE; i++)
	{
		//left wall
		thickenWallInMaze(Left, i, (MSIZE / 2) - (5 * SPACE_SIZE));
		thickenWallInMaze(Right, i, (MSIZE / 2) - (6 * SPACE_SIZE));

		//right wall
		thickenWallInMaze(Right, i, (MSIZE / 2) + (5 * SPACE_SIZE));
		thickenWallInMaze(Left, i, (MSIZE / 2) + (6 * SPACE_SIZE));
	}

	//top left
	setUnreachableZone(2 * MSIZE / 3 - middleDoubleSpaceCells + 1, 2 * MSIZE / 3 + CELL_SIZE,
		(MSIZE / 2) - (6 * SPACE_SIZE) + 1, (MSIZE / 2) - (5 * SPACE_SIZE));
	//top right
	setUnreachableZone(2 * MSIZE / 3 - middleDoubleSpaceCells + 1, 2 * MSIZE / 3 + CELL_SIZE,
		(MSIZE / 2) + (5 * SPACE_SIZE) + 1, (MSIZE / 2) + (6 * SPACE_SIZE));


	// left vertical lines
	for (j = (MSIZE / 2) - (5 * SPACE_SIZE); j > (MSIZE / 2) - (6 * SPACE_SIZE) - CELL_SIZE; j--)
	{
		// top
		thickenWallInMaze(Top, 2 * MSIZE / 3 - middleDoubleSpaceCells, j);
		thickenWallInMaze(Bottom, 2 * MSIZE / 3, j);

		// bottom
		thickenWallInMaze(Top, MSIZE / 3, j);
		thickenWallInMaze(Bottom, MSIZE / 3 + middleDoubleSpaceCells, j);
	}

	// right vertical lines
	for (j = (MSIZE / 2) + (5 * SPACE_SIZE); j < (MSIZE / 2) + (6 * SPACE_SIZE) + CELL_SIZE; j++)
	{
		// top
		thickenWallInMaze(Top, 2 * MSIZE / 3 - middleDoubleSpaceCells, j);    //bottom line
		thickenWallInMaze(Bottom, 2 * MSIZE / 3, j);    //top line

														// bottom
		thickenWallInMaze(Top, MSIZE / 3, j);    //bottom line
		thickenWallInMaze(Bottom, MSIZE / 3 + middleDoubleSpaceCells, j);    //top line
	}

}

void setupTopSection()
{
	int i, j;

	// left +
	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE);

		if ((i <= MSIZE - 2 * SPACE_SIZE && i > MSIZE - 3 * SPACE_SIZE - CELL_SIZE) || (i < MSIZE - 4 * SPACE_SIZE && i > MSIZE - 6 * SPACE_SIZE - CELL_SIZE))
			thickenWallInMaze(Right, i, (MSIZE / 2) - (3 * SPACE_SIZE) - 2 * SPACE_SIZE - SPACE_SIZE);
	}

	for (j = (MSIZE / 2) - (5 * SPACE_SIZE); j >= (MSIZE / 2) - (6 * SPACE_SIZE); j--)
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

	setUnreachableZone(MSIZE - 6 * SPACE_SIZE + 1, MSIZE - 2 * SPACE_SIZE,
		(MSIZE / 2) - (6 * SPACE_SIZE) + 1, (MSIZE / 2) - (5 * SPACE_SIZE));

	setUnreachableZone(MSIZE - 4 * SPACE_SIZE + 1, MSIZE - 3 * SPACE_SIZE - CELL_SIZE,
		2 * SPACE_SIZE + 1, 6 * SPACE_SIZE + CELL_SIZE);

	// right +
	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE);

		if ((i <= MSIZE - 2 * SPACE_SIZE && i > MSIZE - 3 * SPACE_SIZE - CELL_SIZE) || (i < MSIZE - 4 * SPACE_SIZE && i > MSIZE - 6 * SPACE_SIZE - CELL_SIZE))
			thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) + 2 * SPACE_SIZE + SPACE_SIZE);
	}

	for (j = (MSIZE / 2) + (5 * SPACE_SIZE); j <= (MSIZE / 2) + (6 * SPACE_SIZE); j++)
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

	setUnreachableZone(MSIZE - 6 * SPACE_SIZE + 1, MSIZE - 2 * SPACE_SIZE,
		(MSIZE / 2) + (5 * SPACE_SIZE) + 1, (MSIZE / 2) + (6 * SPACE_SIZE));

	setUnreachableZone(MSIZE - 4 * SPACE_SIZE + 1, MSIZE - 3 * SPACE_SIZE - CELL_SIZE,
		MSIZE - 6 * SPACE_SIZE - CELL_SIZE + 1, MSIZE - 2 * SPACE_SIZE);

}

void setupBottomSection()
{
	int i, j;

	// right rectangle
	for (j = MSIZE - 2 * SPACE_SIZE; j > MSIZE - 7 * SPACE_SIZE; j--)
	{
		thickenWallInMaze(Bottom, 7 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, 6 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 7 * SPACE_SIZE - CELL_SIZE; i > 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, MSIZE - 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, MSIZE - 7 * SPACE_SIZE);
	}

	setUnreachableZone(6 * SPACE_SIZE + CELL_SIZE + 1, 7 * SPACE_SIZE - CELL_SIZE, MSIZE - 7 * SPACE_SIZE + 1, MSIZE - 2 * SPACE_SIZE);


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

	setUnreachableZone(3 * SPACE_SIZE + CELL_SIZE + 1, 4 * SPACE_SIZE - CELL_SIZE, MSIZE - 7 * SPACE_SIZE + 1, MSIZE - 2 * SPACE_SIZE);
	setUnreachableZone(2 * SPACE_SIZE + 1, 5 * SPACE_SIZE, MSIZE - 5 * SPACE_SIZE + 1, MSIZE - 4 * SPACE_SIZE);

	// left rectangle
	for (j = 2 * SPACE_SIZE; j < 7 * SPACE_SIZE; j++)
	{
		thickenWallInMaze(Bottom, 4 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, 3 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 4 * SPACE_SIZE - CELL_SIZE; i > 3 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, 7 * SPACE_SIZE);
	}

	setUnreachableZone(3 * SPACE_SIZE + CELL_SIZE + 1, 4 * SPACE_SIZE - CELL_SIZE, 2 * SPACE_SIZE + 1, 7 * SPACE_SIZE);


	// draw left -
	for (j = 2 * SPACE_SIZE; j < 7 * SPACE_SIZE; j++)
	{
		thickenWallInMaze(Bottom, 7 * SPACE_SIZE - CELL_SIZE, j);
		thickenWallInMaze(Top, 6 * SPACE_SIZE + CELL_SIZE, j);
	}

	for (i = 7 * SPACE_SIZE - CELL_SIZE; i > 6 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Right, i, 2 * SPACE_SIZE);
		thickenWallInMaze(Left, i, 7 * SPACE_SIZE);
	}

	setUnreachableZone(6 * SPACE_SIZE + CELL_SIZE + 1, 7 * SPACE_SIZE - CELL_SIZE, 2 * SPACE_SIZE + 1, 7 * SPACE_SIZE);
}

void setupInitials()
{
	int i, j;

	for (i = MSIZE - 2 * SPACE_SIZE; i > MSIZE - 8 * SPACE_SIZE; i--)
	{
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE));
		thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) + CELL_SIZE);

		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE));
		thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) - CELL_SIZE);

		for (j = MSIZE / 2 - 3 * SPACE_SIZE; j < MSIZE / 2 + 3 * SPACE_SIZE; j++)
		{
			if (i == MSIZE - 5 * SPACE_SIZE)
			{
				thickenWallInMaze(Bottom, i, j);
				thickenWallInMaze(Bottom, i + CELL_SIZE, j);
				thickenWallInMaze(Bottom, i - CELL_SIZE, j);
			}

		}
	}

	for (i = 2 * SPACE_SIZE; i <= 8 * SPACE_SIZE; i++)
	{
		if (i < 5 * SPACE_SIZE)
		{
			thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) - CELL_SIZE);
			thickenWallInMaze(Left, i, (MSIZE / 2) + (3 * SPACE_SIZE) - 2 * CELL_SIZE);
		}
		else if (i < 8 * SPACE_SIZE)
		{
			thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE));
			thickenWallInMaze(Left, i, (MSIZE / 2) - (3 * SPACE_SIZE) + CELL_SIZE);
		}

		for (j = MSIZE / 2 - 3 * SPACE_SIZE; j < MSIZE / 2 + 3 * SPACE_SIZE; j++)
		{
			if (i == 2 * SPACE_SIZE || i == 5 * SPACE_SIZE || i == 8 * SPACE_SIZE)
			{
				thickenWallInMaze(Bottom, i, j);
				thickenWallInMaze(Bottom, i - CELL_SIZE, j);
			}
		}
	}
}


void setupMaze()
{
	int i, j;

	//clean up the maze
	for (i = 0; i < MSIZE; i++/*= CELL_SIZE*/)
		for (j = 0; j < MSIZE; j++/*= CELL_SIZE*/)
			maze[i][j] = SPACE;

	setupPerimeter();
	setupCenterSquare();
	setupInitials();
	setupCenterWalls();
	setupBottomSection();
	setupTopSection();
}

/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

bool checkPointCloseToWallVerticalUp(int row, int col, int range = 2 * CELL_SIZE)
{
	col -= range;

	for (int i = row; i < row + range && i < MSIZE; i++)
		for (int j = col; j < col + 2 * range && j < MSIZE; j++)
			if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
				return false;

	return true;
}


bool checkPointCloseToWallHorizontalRight(int row, int col, int range = 2 * CELL_SIZE)
{
	row -= range;

	for (int i = row; i < row + 2 * range && i < MSIZE; i++)
		for (int j = col; j < col + range && j < MSIZE; j++)
			if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
				return false;


	return true;
}

bool checkPointCloseToWallForMonster(int row, int col)
{
	int range = 2 * CELL_SIZE;

	//don't place coin in that case
	if (row - range <= 0 || col - range <= 0)
		return false;

	row -= range;
	col -= range;

	for (int i = row; i < row + 2 * range && i < MSIZE; i++)
		for (int j = col; j < col + 2 * range && j < MSIZE; j++)
			if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE)
				return false;


	return true;
}

bool checkPointCloseToWall(int row, int col)
{
	int range = 2 * CELL_SIZE;

	//don't place coin in that case
	if (row - range <= 0 || col - range <= 0)
		return false;

	row -= range;
	col -= range;

	for (int i = row; i < row + 2 * range && i < MSIZE; i++)
		for (int j = col; j < col + 2 * range && j < MSIZE; j++)
			if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
				return false;


	return true;
}

void storeCurrentPointInParrentArray(int row, int col, Point2D* parentPoint, Point2D* parentArray[][MSIZE], vector<Point2D*> &grayArray)
{
	Point2D* ptAddToGray = new Point2D(col, row);
	parentArray[row][col] = parentPoint;
	grayArray.push_back(ptAddToGray);
	coins_locations.push_back(ptAddToGray);
}

bool setPointAsGray(int mazeRow, int mazeCol, Point2D*& parentPoint, Point2D* parentArray[][MSIZE], vector<Point2D*> &grayVector)
{
	if ((maze[mazeRow][mazeCol] == SPACE) && checkPointCloseToWall(mazeRow, mazeCol))
	{
		maze[mazeRow][mazeCol] = COIN;    //add it to gray
		coinCounter++;
		storeCurrentPointInParrentArray(mazeRow, mazeCol, parentPoint, parentArray, grayVector);
		return true;
	}

	return false;
}

void bfsIteration()
{
	Point2D* pt;
	int mazeRow, mazeCol;

	if (gray_coins.empty() || coinCounter >= MAX_COINS)    //grey is the edges that didn't visited yet
		coinsBfs = false;    //there is no path to the target

	else
	{
		pt = gray_coins[0];    //this will be the parent
		gray_coins.erase(gray_coins.begin());    //deque

		mazeRow = pt->getY();
		mazeCol = pt->getX();

		//paint pt VISITED
		if ((mazeRow == MSIZE - (startPointForCoins_y)) && (mazeCol == MSIZE - (startPointForCoins_x)))    //found target
		{
			storeCurrentPointInParrentArray(mazeRow, mazeCol, pt, parent_forCoins, gray_coins);
			return;
		}

		else
		{
			if (maze[mazeRow][mazeCol] == SPACE)
				maze[mazeRow][mazeCol] = COIN;    //y is i, x is j


			mazeRow = pt->getY() + COINS_STEP;
			setPointAsGray(mazeRow, mazeCol, pt, parent_forCoins, gray_coins);

			mazeRow = pt->getY() - COINS_STEP;
			setPointAsGray(mazeRow, mazeCol, pt, parent_forCoins, gray_coins);

			mazeRow = pt->getY();
			mazeCol = pt->getX() + COINS_STEP;
			setPointAsGray(mazeRow, mazeCol, pt, parent_forCoins, gray_coins);

			mazeCol = pt->getX() - COINS_STEP;
			setPointAsGray(mazeRow, mazeCol, pt, parent_forCoins, gray_coins);


		}
	}
}

void setupCoins()
{
	while (coinsBfs)
		bfsIteration();
}

bool checkForWallsBetweenTwoPoints(Point2D* startPoint, Point2D* targetPoint)
{
	int target_x = targetPoint->getX();
	int target_y = targetPoint->getY();

	int nextCoin_x = startPoint->getX();
	int nextCoin_y = startPoint->getY();

	if (target_y <= nextCoin_y) // coin top
	{
		for (int i = target_y; i <= nextCoin_y; i++)
		{
			if (target_x <= nextCoin_x) // coin right
			{
				for (int j = target_x; j <= nextCoin_x; j++)
					if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
						return false;
			}
			else // coin left
			{
				for (int j = target_x; j >= nextCoin_x; j--)
					if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
						return false;
			}
		}
	}
	else // coin top
	{
		for (int i = target_y; i >= nextCoin_y; i--)
		{
			if (target_x <= nextCoin_x) // coin right
			{
				for (int j = target_x; j <= nextCoin_x; j++)
					if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
						return false;
			}
			else  // coin left
			{
				for (int j = target_x; j >= nextCoin_x; j--)
					if (maze[i][j] == WALL || maze[i][j] == UNREACHABLE || maze[i][j] == CENTER)
						return false;
			}
		}
	}

	return true;
}

Monster* findNearestMonsterToPacman()
{
	Point2D* pacmanLoc = pacman.getPacmanLocation();
	double tmpDist = pacmanLoc->getDistanceFromPoint(monster1.getLocation());
	Monster* nearestMonsterToPacman = &monster1;
	
	if (pacmanLoc->getDistanceFromPoint(monster2.getLocation()) < tmpDist)
	{
		tmpDist = pacmanLoc->getDistanceFromPoint(monster2.getLocation());
		nearestMonsterToPacman = &monster2;
	}

	if (pacmanLoc->getDistanceFromPoint(monster3.getLocation()) < tmpDist)
		nearestMonsterToPacman = &monster3;

	return nearestMonsterToPacman;

}

//method wiil return true if found a coin in the vector, otherwise it returns false
bool findNearestCoin()
{

	if (coins_locations.empty())
	{
		nearestCoinToPacman = nullptr;
		pacmanWon = true;
		return false;
	}

	if (nearestCoinToPacman && maze[nearestCoinToPacman->getY()][nearestCoinToPacman->getX()] == TARGET_COIN)
		maze[nearestCoinToPacman->getY()][nearestCoinToPacman->getX()] = COIN;

	Point2D* nearestMonster = findNearestMonsterToPacman()->getLocation();
	double maxDistCoinFromMonster = 0.0;
	double tmpDistFromCoinToMonster;

	random_shuffle(coins_locations.begin(), coins_locations.end());
		Point2D* pacmanLocation = pacman.getPacmanLocation();
		nearestCoinToPacman = coins_locations[0];
		double minDistance = MSIZE * 2;
		double tmpDist = 0.0;
		Point2D* tmpCoinLoc;
		int indexToDelete = 0;

		for (int i = 1; i < coins_locations.size(); i++)
		{
			tmpCoinLoc = coins_locations[i];
			tmpDist = pacmanLocation->getDistanceFromPoint(tmpCoinLoc);
			tmpDistFromCoinToMonster = nearestMonster->getDistanceFromPoint(tmpCoinLoc);

			if (!checkForWallsBetweenTwoPoints(pacman.getPacmanLocation(), tmpCoinLoc))
				tmpDist += 20 * SPACE_SIZE;

			if (!checkForWallsBetweenTwoPoints(tmpCoinLoc, nearestMonster))
				tmpDistFromCoinToMonster += 20 * SPACE_SIZE;

			if ((tmpDist < minDistance) && (tmpDistFromCoinToMonster > 200))
			{
				
				if (find(nearestCoinsVector.begin(), nearestCoinsVector.end(), tmpCoinLoc) == nearestCoinsVector.end())
				{
					minDistance = tmpDist;
					maxDistCoinFromMonster = tmpDistFromCoinToMonster;
					nearestCoinToPacman = tmpCoinLoc;
					indexToDelete = i;
				}
			}
		}

	if (maze[nearestCoinToPacman->getY()][nearestCoinToPacman->getX()] == COIN)
		maze[nearestCoinToPacman->getY()][nearestCoinToPacman->getX()] = TARGET_COIN;

	nearestCoinsVector.push_back(nearestCoinToPacman);


	return true;

}

void initMonsters()
{
	
	monster1_startPoint->set_f(monster1_startPoint, 0);
	monster1_grayPQ.push(monster1_startPoint);

	monster2_startPoint->set_f(monster2_startPoint, 0);
	monster2_grayPQ.push(monster2_startPoint);

	monster3_startPoint->set_f(monster3_startPoint, 0);
	monster3_grayPQ.push(monster3_startPoint);
}

void init()
{
	srand(time(0));
	setupMaze();

	//coins init
	gray_coins.push_back(new Point2D(startPointForCoins_x, startPointForCoins_y));
	coinsBfs = true;
	setupCoins();

	//pacman init
	startPacmanCoinSearch = true;
	pacmanStartPoint->set_f(pacmanStartPoint, 0);
	pacmanRoute.push(pacmanStartPoint);

	findNearestCoin();

	initMonsters();

	

	glClearColor(0.7, 0.7, 0.7, 0);
	glOrtho(0, MSIZE, 0, MSIZE, 0, MSIZE);    //regular origin!!! y from bottom to up, x from left to right

}



void storeCurrentPointForAstar(int row, int col, Point2D*& parentPoint, Point2D*& targetPoint)
{
	Point2D* ptAddToGray = new Point2D(col, row);
	ptAddToGray->set_f(targetPoint, parentPoint->get_g() + 1);

	parentPoint->set_f(pacman.getPacmanLocation(), parentPoint->getX() + parentPoint->getY());

	if(!parent_pacman[row][col])
		parent_pacman[row][col] = parentPoint;

	if (pacmanMaze[row][col] != PACMAN_VISITED)
		pacmanRoute.push(ptAddToGray);
}

void setPointAsGrayForAStar(int mazeRow, int mazeCol, Point2D*& parentPoint, Point2D*& targetPoint)
{
	if (maze[mazeRow][mazeCol] == TARGET_COIN || maze[mazeRow][mazeCol] == COIN || maze[mazeRow][mazeCol] == SPACE /*|| maze[mazeRow][mazeCol] == PACMAN_VISITED*/)    //found target
		storeCurrentPointForAstar(mazeRow, mazeCol, parentPoint, targetPoint);
}




bool checkRouteVertical(Point2D* parentPoint)
{
	int mazeRow = parentPoint->getY();
	int mazeCol = parentPoint->getX();

	if (!checkVertical)
	{
		if (checkPointCloseToWall(rowToCheck, mazeCol))
		{
			setPointAsGrayForAStar(rowToCheck, mazeCol, parentPoint, nearestCoinToPacman);
			checkVertical = true;
			return true;
		}
		else
			return false;

	}

	if (checkPointCloseToWall(mazeRow, mazeCol))
	{

		//check up
		mazeRow = parentPoint->getY() + 1;
		setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);

		//check down
		mazeRow = parentPoint->getY() - 1;
		setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);

		return true;
	}
	else //stuck from up or down
	{
		if (!checkPointCloseToWallVerticalUp(mazeRow, mazeCol))    //stuck from up
		{

				mazeRow--;
				if (pacmanMaze[mazeRow][mazeCol] != PACMAN_VISITED)
				{
					if (checkPointCloseToWallVerticalUp(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
				}
				else
				{
					checkVertical = false;
					checkHorizontal = true;
					rowToCheck = mazeRow + 1;

					if (checkPointCloseToWallHorizontalRight(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
					else
					{
						mazeCol--;
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;

					}

					return false;
				}
			}
		else    //stuck from down
		{

				mazeRow++;
				if (pacmanMaze[mazeRow][mazeCol] != PACMAN_VISITED)
				{
					if (checkPointCloseToWallVerticalUp(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
				}
				else
				{
					checkVertical = false;
					checkHorizontal = true;
					rowToCheck = mazeRow - 1;

					if (checkPointCloseToWallHorizontalRight(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
					else
					{
						mazeCol--;
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;

					}
					return false;
				}
			}
		}
	return false;
}

bool checkRouteHorizontal(Point2D* parentPoint)
{
	int mazeRow = parentPoint->getY();
	int mazeCol = parentPoint->getX();

	if (!checkHorizontal)
	{
		if (checkPointCloseToWall(mazeRow, colToCheck))
		{
			setPointAsGrayForAStar(mazeRow, colToCheck, parentPoint, nearestCoinToPacman);
			checkHorizontal = true;
			return true;
		}
		else
			return false;

	}

	if (checkPointCloseToWall(mazeRow, mazeCol))
	{
		//check right
		mazeCol = parentPoint->getX() + 1;
		setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);

		//check left
		mazeCol = parentPoint->getX() - 1;
		setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);

		return true;
	}
	else
	{
		if (!checkPointCloseToWallHorizontalRight(mazeRow, mazeCol))    //stuck from right
		{
		
				mazeCol--;
				if (pacmanMaze[mazeRow][mazeCol] != PACMAN_VISITED)
				{
					if (checkPointCloseToWallHorizontalRight(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
				}
				else
				{
					checkVertical = true;
					checkHorizontal = false;
					colToCheck = mazeCol + 1;

					if (checkPointCloseToWallVerticalUp(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
					else
					{
						mazeRow--;
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;

					}

					return false;
				}
			}
		else    //stuck from left
		{

				mazeCol++;
				if (pacmanMaze[mazeRow][mazeCol] != PACMAN_VISITED)
				{
					if (checkPointCloseToWallHorizontalRight(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
				}
				else
				{
					checkVertical = true;
					checkHorizontal = false;
					colToCheck = mazeCol - 1;

					if (checkPointCloseToWallVerticalUp(mazeRow, mazeCol))
					{
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;
					}
					else
					{
						mazeRow--;
						setPointAsGrayForAStar(mazeRow, mazeCol, parentPoint, nearestCoinToPacman);
						return true;

					}

					return false;
				}
			}
		}

	//}
	return false;
}

void getPacmanPathToMove(Point2D* point)
{
	Point2D* tmpPoint = point;
	while (tmpPoint != NULL && *tmpPoint != *pacman.getPacmanLocation())
	{
		pacmanPath.push_back(tmpPoint);
		tmpPoint = parent_pacman[tmpPoint->getY()][tmpPoint->getX()];
	}
}

void clearMazeFromPacmanPath()
{
	for (int i = 0; i < MSIZE; i++)
		for (int j = 0; j < MSIZE; j++)
		{
			if (pacmanMaze[i][j] > 0)
				pacmanMaze[i][j] = 0;
			if (parent_pacman[i][j])
				parent_pacman[i][j] = nullptr;

		}
}

void a_starIterationForPacman()
{
	Point2D* pt = nullptr;
	int mazeRow, mazeCol;

	if (pacmanRoute.empty())    
		startPacmanCoinSearch = false;    //there is no path to the target
	

	else
	{
		pt = pacmanRoute.top();    //this will be the parent
		pacmanRoute.pop();
		lastPointForPath = pt;

		mazeRow = pt->getY();
		mazeCol = pt->getX();

		//paint pt VISITED
		if (maze[mazeRow][mazeCol] == TARGET_COIN)    //found target
		{
			storeCurrentPointForAstar(mazeRow, mazeCol, pt, nearestCoinToPacman);
			startPacmanCoinSearch = false;
	
		}

		else 
		{
			pacmanMaze[mazeRow][mazeCol] = PACMAN_VISITED;

			int nearestCoin_x = nearestCoinToPacman->getX();
			int nearestCoin_y = nearestCoinToPacman->getY();

			if (mazeCol != nearestCoin_x && mazeRow != nearestCoin_y)
			{
				if (checkVertical)
				{
					if (!checkRouteVertical(pt))
						checkRouteHorizontal(pt);
				}
				else
				{
					checkRouteHorizontal(pt);
					checkRouteVertical(pt);
				}
			}
			else if (mazeCol != nearestCoin_x)
			{
				if (checkHorizontal)
				{
					if (!checkRouteHorizontal(pt))
						checkRouteVertical(pt);
				}
				else
				{
					checkRouteVertical(pt);
					checkRouteHorizontal(pt);
				}
			}
			else if (mazeRow != nearestCoin_y)    // in case that pacman and the coin isn't on the same row (y value)
			{
				if (checkVertical)
				{
					if (!checkRouteVertical(pt))
						checkRouteHorizontal(pt);
				}
				else
				{
					checkRouteHorizontal(pt);
					checkRouteVertical(pt);
				}
			}
		}
	}

	if (!startPacmanCoinSearch || pacman_Astar_Counter % 600 == 0)	//target was found
	{
		startPacmanCoinSearch = false;
		getPacmanPathToMove(lastPointForPath);
		clearMazeFromPacmanPath();

		int size = pacmanRoute.size();

		for (int i = 0; i < size; i++)
			pacmanRoute.pop();
		lastPointForPath->set_f(lastPointForPath, 0);
		pacmanRoute.push(lastPointForPath);
	}
}

bool isMonsterCaughtPacman(int mazeRow, int mazeCol)
{
	
	if (mazeRow == pacman.getPacmanLocation()->getY() && mazeCol == pacman.getPacmanLocation()->getX())
		return true;

	return false;
}

bool isMonsterInCenterSquare(Monster* monster)
{
	int row = monster->getLocation()->getY();
	int col = monster->getLocation()->getX();

	if (row < (MSIZE / 2) + SPACE_SIZE && row >(MSIZE / 2) - SPACE_SIZE &&
		col < (MSIZE / 2) + (3 * SPACE_SIZE) && col >(MSIZE / 2) - (3 * SPACE_SIZE))	//in the center
		return true;

	return false;
}

void storeCurrentPointForAstar_Monster(int row, int col, Point2D* parentPoint, Point2D* parent_monster[][MSIZE], 
	priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist>& monsterGrayPQ, Point2D* monsterTargetPoint)
{
	
	Point2D* ptAddToGray = new Point2D(col, row);
	ptAddToGray->set_f(monsterTargetPoint, parentPoint->get_g() + 1);
	if(!parent_monster[row][col])
		parent_monster[row][col] = parentPoint;
	monsterGrayPQ.push(ptAddToGray);
}

//returns true if this monster has visited that cell, it means it's already been taken by him
bool hasMonsterVisitedCell(Monster* monster, int mazeRow, int mazeCol)
{
	if (monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED + MONSTER2_VISITED + MONSTER3_VISITED)
		return true;

	if (monster == &monster1)
	{
		if (monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED + MONSTER2_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED + MONSTER3_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED)

			return true;
	}

	if (monster == &monster2)
	{
		if (monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED + MONSTER2_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER2_VISITED + MONSTER3_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER2_VISITED)

			return true;
	}

	if (monster == &monster3)
	{
		if (monstersMaze[mazeRow][mazeCol] == MONSTER1_VISITED + MONSTER3_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER2_VISITED + MONSTER3_VISITED ||
			monstersMaze[mazeRow][mazeCol] == MONSTER3_VISITED)

			return true;
	}

	return false;
}

void setPointAsGrayForAStarForMonster(int mazeRow, int mazeCol, Point2D* parentPoint, Point2D* parent_monster[][MSIZE],
	priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist>& monsterGrayPQ, Monster* monster, Point2D* monsterTargetPoint, 
	int monsterNumVisited)
{
	int monster_x = monster->getLocation()->getX();
	int monster_y = monster->getLocation()->getY();

	if (checkPointCloseToWallForMonster(mazeRow, mazeCol))
	{

		if (isMonsterCaughtPacman(mazeRow, mazeCol))	//found target			
			storeCurrentPointForAstar_Monster(mazeRow, mazeCol, parentPoint, parent_monster, monsterGrayPQ, monsterTargetPoint);
		else if(!isMonsterInCenterSquare(monster))
		{
			if (maze[mazeRow][mazeCol] != WALL && maze[mazeRow][mazeCol] != UNREACHABLE && !hasMonsterVisitedCell(monster, mazeRow, mazeCol)/*monstersMaze[mazeRow][mazeCol] == 0*/
				&& maze[mazeRow][mazeCol] != CENTER)
				if (mazeCol != monster_x || mazeRow != monster_y)
				{
					monstersMaze[mazeRow][mazeCol] += monsterNumVisited;
					storeCurrentPointForAstar_Monster(mazeRow, mazeCol, parentPoint, parent_monster, monsterGrayPQ, monsterTargetPoint);
				}
		}
		else
		{
			if (maze[mazeRow][mazeCol] != WALL && maze[mazeRow][mazeCol] != UNREACHABLE && !hasMonsterVisitedCell(monster, mazeRow, mazeCol) /*monstersMaze[mazeRow][mazeCol] == 0*/)
				if (mazeCol != monster_x || mazeRow != monster_y)
				{
					monstersMaze[mazeRow][mazeCol] += monsterNumVisited;
					storeCurrentPointForAstar_Monster(mazeRow, mazeCol, parentPoint, parent_monster, monsterGrayPQ, monstersEscapePointFromCenter);
				}

		}
	}
}

void getMonsterPathToMove(Point2D* point, Monster* monster, Point2D* parent_monster[MSIZE][MSIZE], vector<Point2D*>& monster_path)
{
	Point2D* tmpPoint = point;
	while (tmpPoint != NULL && *tmpPoint != *monster->getLocation())
	{
			monster_path.push_back(tmpPoint);
		tmpPoint = parent_monster[tmpPoint->getY()][tmpPoint->getX()];
	}
}

void clearMazeFromMonsterPath(int monsterVisited, Point2D* parentMonster[][MSIZE], Monster* monster)
{
	for (int i = 0; i < MSIZE; i++)
		for (int j = 0; j < MSIZE; j++)
		{
			if (hasMonsterVisitedCell(monster, i, j))
				monstersMaze[i][j] -= monsterVisited;
			if (parentMonster[i][j])
				parentMonster[i][j] = nullptr;

		}
}


void moveMonster(Monster* monster, Point2D* parent_monster[][MSIZE], vector<Point2D*>& monster_path, int monsterNumVisited, bool& isPacmanCaught)
{
	int monster_x = monster->getLocation()->getX();
	int monster_y = monster->getLocation()->getY();

	int pacmanScale_xMax = pacman.getPacmanLocation()->getX() + pacman.getScale() / 2;
	int pacmanScale_xMin = pacman.getPacmanLocation()->getX() - pacman.getScale() / 2;
	int pacmanScale_yMax = pacman.getPacmanLocation()->getY() + pacman.getScale() / 2;
	int pacmanScale_yMin = pacman.getPacmanLocation()->getY() - pacman.getScale() / 2;

	if ((monster_x >= pacmanScale_xMin && monster_x <= pacmanScale_xMax)
		&& (monster_y >= pacmanScale_yMin && monster_y <= pacmanScale_yMax))
	{
		gameOver = true;
	}
	else if (!monster_path.empty())
	{
		Point2D* nextMove = monster_path[monster_path.size() -1];
		int nextMove_x = nextMove->getX();
		int nextMove_y = nextMove->getY();
		double pacmanAngle;

		monster_path.erase(monster_path.end() - 1);

		monster->setTranslation(new Point2D(nextMove_x, nextMove_y));

	}
	else
	{
		isPacmanCaught = false;
		clearMazeFromMonsterPath(monsterNumVisited, parent_monster, monster);
	}

}


void a_starIterationForMonster(priority_queue<Point2D*, vector<Point2D*>, ComparePointsByDist>& monsterGrayPQ,
	Point2D* parent_monster[MSIZE][MSIZE], Monster* monster, vector<Point2D*>& monster_path, Point2D* monsterTargetPoint,
	int monsterNum_Visited, bool& isPacmanCaught)
{
	Point2D* pt = nullptr;
	int mazeRow, mazeCol;

	if (monsterGrayPQ.empty())	//grey is the edges that didn't visited yet
		isPacmanCaught = true;	//there is no path to the target
	else
	{
		pt = monsterGrayPQ.top();	//this will be the parent
		monsterGrayPQ.pop();

		mazeRow = pt->getY();
		mazeCol = pt->getX();

		if (isMonsterCaughtPacman(mazeRow, mazeCol))	//found target	
			isPacmanCaught = true;
		

		else
		{
			if (mazeRow == monster->getLocation()->getY() && mazeCol == monster->getLocation()->getX())
				if (checkPointCloseToWallForMonster(mazeRow, mazeCol) && !hasMonsterVisitedCell(monster, mazeRow, mazeCol))
					monstersMaze[mazeRow][mazeCol] += monsterNum_Visited;

			//check up
			mazeRow = pt->getY() + 1;
			setPointAsGrayForAStarForMonster(mazeRow, mazeCol, pt, parent_monster, monsterGrayPQ, monster, monsterTargetPoint, monsterNum_Visited);

			//check down
			mazeRow = pt->getY() - 1;
			setPointAsGrayForAStarForMonster(mazeRow, mazeCol, pt, parent_monster, monsterGrayPQ, monster, monsterTargetPoint, monsterNum_Visited);

			//check right
			mazeRow = pt->getY();
			mazeCol = pt->getX() + 1;
			setPointAsGrayForAStarForMonster(mazeRow, mazeCol, pt, parent_monster, monsterGrayPQ, monster, monsterTargetPoint, monsterNum_Visited);

			//check left
			mazeCol = pt->getX() - 1;
			setPointAsGrayForAStarForMonster(mazeRow, mazeCol, pt, parent_monster, monsterGrayPQ, monster, monsterTargetPoint, monsterNum_Visited);

			
		}
	}

	int counter = 0;
	if (monster == &monster1)
		counter = monster_Astar_Counter_Arr[0];
	else if (monster == &monster2)
		counter = monster_Astar_Counter_Arr[1];
	else if (monster == &monster3)
		counter = monster_Astar_Counter_Arr[2];

	if (isPacmanCaught || counter % 300 == 0)	//target was found
	{
		isPacmanCaught = true;
		getMonsterPathToMove(pt, monster, parent_monster, monster_path);
		int size = monsterGrayPQ.size();

		for (int i = 0; i < size; i++)
			monsterGrayPQ.pop();
		monsterGrayPQ.push(pt);
	}
}


//drawing the maze 600x600px - MSIZExMSIZE, setting the color for each pixel in the maze, and displaying it
void drawMaze()
{
	int i, j;
	double m2scr = (2.0) / MSIZE;

	for (i = 0; i < MSIZE; i += CELL_SIZE)
	{
		for (j = 0; j < MSIZE; j += CELL_SIZE)
		{
			switch (maze[i][j])
			{
			case WALL:
				glColor3d(0.2, 0, 1);    //purple
				break;
			case SPACE:
				glColor3d(0, 0, 0);    //black
				break;
			case COIN:
				glColor3d(1, 1, 1);
				break;
			case CENTER:
				glColor3d(0, 0, 0);
				break;
			case TARGET_COIN:
				glColor3d(1, 1, 1);
				break;
			default:
				glColor3d(0, 0, 0);
				break;

			}

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j + CELL_SIZE, i);
			glVertex2d(j + CELL_SIZE, i + CELL_SIZE);
			glVertex2d(j, i + CELL_SIZE);
			glEnd();

		}
	}

}



void movePacmanToCoin()
{
	int pacman_x = pacman.getPacmanLocation()->getX();
	int pacman_y = pacman.getPacmanLocation()->getY();

	if (maze[pacman_y][pacman_x] == TARGET_COIN)
	{
		maze[pacman_y][pacman_x] = SPACE;
		if (!pacmanRoute.empty())
		{
			int size = pacmanRoute.size();
			for (int i = 0; i < size; i++)
				pacmanRoute.pop();
		}

		vector<Point2D*>::iterator itr = find(coins_locations.begin(), coins_locations.end(), nearestCoinToPacman);
		coins_locations.erase(itr);
		nearestCoinsVector.erase(nearestCoinsVector.begin(), nearestCoinsVector.end());

		startPacmanCoinSearch = true;
		findNearestCoin();
		pacman.getPacmanLocation()->set_f(nearestCoinToPacman, 0);
		pacmanRoute.push(pacman.getPacmanLocation());
		
	}
	else if (!pacmanPath.empty())
	{
		Point2D* nextMove = pacmanPath[pacmanPath.size() - 1];
		int nextMove_x = nextMove->getX();
		int nextMove_y = nextMove->getY();

		pacmanPath.erase(pacmanPath.end() - 1);
		PacmanPawn::pacmanDirection pacDir = pacman.getDirection();

		if (pacman_x < nextMove_x)
			pacDir = PacmanPawn::pacmanDirection::Right;
		else if (pacman_x > nextMove_x)
			pacDir = PacmanPawn::pacmanDirection::Left;
		else if (pacman_y < nextMove_y)
			pacDir = PacmanPawn::pacmanDirection::Up;
		else if (pacman_y > nextMove_y)
			pacDir = PacmanPawn::pacmanDirection::Down;

		pacman.setTranslation(pacDir, new Point2D(nextMove_x, nextMove_y));

		if (maze[pacman_y][pacman_x] == TARGET_COIN)
		{
			vector<Point2D*>::iterator itr = find(coins_locations.begin(), coins_locations.end(), nearestCoinToPacman);
			coins_locations.erase(itr);
		}
		else if (maze[pacman_y][pacman_x] == COIN)
		{
			Point2D* tmpPoint = new Point2D(pacman_x, pacman_y);
			vector<Point2D*>::iterator itr = find(coins_locations.begin(), coins_locations.end(), tmpPoint);
			if(itr != coins_locations.end())
				coins_locations.erase(itr);

		}
		maze[pacman_y][pacman_x] = SPACE;

	}
	else
	{
		startPacmanCoinSearch = true;
		findNearestCoin();
		pacman.getPacmanLocation()->set_f(nearestCoinToPacman, 0);
		pacmanRoute.push(pacman.getPacmanLocation());
	}


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	

	if (!gameOver && !pacmanWon)
	{
		glPushMatrix();
		drawMaze();
		glPopMatrix();

		pacman.drawPacman();
		monster1.drawMonster();
		monster2.drawMonster();
		monster3.drawMonster();
	}
	else if(gameOver)
	{
		glRasterPos2d(0, 0);
		glDrawPixels(MSIZE, MSIZE, GL_RGB, GL_UNSIGNED_BYTE, picture);
	}
	else
	{

	}
	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void cleanUpPacmanMaze()
{
	for (int i = 0; i < MSIZE; i++)
		for (int j = 0; j < MSIZE; j++)
			if (pacmanMaze[i][j] == PACMAN_VISITED)
				pacmanMaze[i][j] = SPACE;
}

void setGameOver()
{
	unsigned char* bmp= nullptr;

	FILE* pf;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	int sz;

	pf = fopen("gameOver.bmp", "rb");	//read binary fie

	fread(&bf, sizeof(bf), 1, pf);
	fread(&bi, sizeof(bi), 1, pf);
	sz = bi.biHeight * bi.biWidth * 3;	//3 colors - RGB

	if (bmp)
		free(bmp);

	bmp = (unsigned char*)malloc(sz);

	fread(bmp, 1, sz, pf);
	fclose(pf);


	int i, j;
	sz = MSIZE * MSIZE * 3;

	for (int k = 0, j = 0, i = 0; k < sz; k += 3)
	{
		picture[i][j][2] = bmp[k];		//blue
		picture[i][j][1] = bmp[k + 1];	//green
		picture[i][j][0] = bmp[k + 2];	//red
		j++;

		//fill the next line
		if (j == MSIZE)
		{
			j = 0;
			i++;
		}
	}


}

void setPacmanWon()
{

	unsigned char* bmp = nullptr;

	FILE* pf;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	int sz;

	pf = fopen("pacmanWon.bmp", "rb");	//read binary fie

	fread(&bf, sizeof(bf), 1, pf);
	fread(&bi, sizeof(bi), 1, pf);
	sz = bi.biHeight * bi.biWidth * 3;	//3 colors - RGB

	if (bmp)
		free(bmp);

	bmp = (unsigned char*)malloc(sz);

	fread(bmp, 1, sz, pf);
	fclose(pf);


	int i, j;
	sz = MSIZE * MSIZE * 3;

	for (int k = 0, j = 0, i = 0; k < sz; k += 3)
	{
		picture[i][j][2] = bmp[k];		//blue
		picture[i][j][1] = bmp[k + 1];	//green
		picture[i][j][0] = bmp[k + 2];	//red
		j++;

		//fill the next line
		if (j == MSIZE)
		{
			j = 0;
			i++;
		}
	}
}


void idle()
{
	if (!coinsBfs)
	{
		if (!gameOver && !pacmanWon)
		{
			stepCounter++;

			cout << "in idle, startPacmanRun = " << startPacmanCoinSearch << endl;
			if (startPacmanCoinSearch)
			{
				pacman_Astar_Counter++;
				if (find(coins_locations.begin(), coins_locations.end(), nearestCoinToPacman) == coins_locations.end())
					findNearestCoin();
				a_starIterationForPacman();

			}
			else
			{
				cleanUpPacmanMaze();
				checkHorizontal = true;
				checkVertical = true;

				if (stepCounter % 2 == 0)
					movePacmanToCoin();
				if (stepCounter % 40 == 0)
					pacman.changeIsOpen();

			}

			if (!isPacmanCaught1)
			{
				monster_Astar_Counter_Arr[0]++;
				a_starIterationForMonster(monster1_grayPQ, parent_monster1, &monster1, monster1_path, pacman.getPacmanLocation(), MONSTER1_VISITED, isPacmanCaught1);
			}
			else
			{
				if (stepCounter % 2 == 0)
				moveMonster(&monster1, parent_monster1, monster1_path, MONSTER1_VISITED, isPacmanCaught1);
			}

			if (!isPacmanCaught2)
			{
				monster_Astar_Counter_Arr[1]++;
				a_starIterationForMonster(monster2_grayPQ, parent_monster2, &monster2, monster2_path, nearestCoinToPacman, MONSTER2_VISITED, isPacmanCaught2);
			}
			else
			{
				if (stepCounter % 4 == 0)
				moveMonster(&monster2, parent_monster2, monster2_path, MONSTER2_VISITED, isPacmanCaught2);
			}
			 
			if (!isPacmanCaught3)
			{
				monster_Astar_Counter_Arr[2]++;
				a_starIterationForMonster(monster3_grayPQ, parent_monster3, &monster3, monster3_path, pacman.getPacmanLocation(), MONSTER3_VISITED, isPacmanCaught3);
			}
			else
			{
				if (stepCounter % 6 == 0)
				moveMonster(&monster3, parent_monster3, monster3_path, MONSTER3_VISITED, isPacmanCaught3);
			}
		}
		else if (gameOver)
			setGameOver();
		else
			setPacmanWon();
	}

	glutPostRedisplay();// calls indirectly to display
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Pacman");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutMainLoop();
	return 0;

}
