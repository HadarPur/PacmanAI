#pragma once
#ifndef _MOSTER_H
#define _MOSTER_H

#include <vector>
#include "GLUT.h"

#include "Point2D.h"

using namespace std;

class Monster
{
	vector<double>color;
	Point2D* translationPoint;
	double scale;

public:
	Monster(vector<double> color, Point2D* startPoint, double scale);
	~Monster();

	void drawMonster();
	void setTranslation(Point2D* toPoint);
	Point2D* getLocation();

};


#endif // !_MOSTER_H
