#pragma once
#ifndef _PACMANPAW_H
#define _PACMANPAW_H

#include "GLUT.h"
#include <vector>
#include <math.h>
#include "Point2D.h"

using namespace std;



class PacmanPawn
{

	

private:
	bool isOpen = false;
	Point2D* translationPoint;
	int scale;
	int rotationAngle = 0;
	void drawPacmanClosed();
	void drawPacmanOpen();

public:
	enum pacmanDirection { Up, Down, Left, Right };

	PacmanPawn(Point2D* startPoint, int scale);
	~PacmanPawn();

	void setIsOpen(bool isOpen);
	void changeIsOpen();
	void drawPacman();
	Point2D* getPacmanLocation();
	void setTranslation(pacmanDirection dir, Point2D* toPoint);
	int getScale();

	pacmanDirection getDirection();
	
};


#endif // !_PACMANPAW_H
