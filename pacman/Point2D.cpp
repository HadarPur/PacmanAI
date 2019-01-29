#include "Point2D.h"



Point2D::Point2D(int x, int y)
{
	this->x = x;
	this->y = y;
	this->g = 0;
	this->h = 0;
}


int Point2D::getX()
{
	return this->x;
}

void Point2D::setX(int x)
{
	this->x = x;
}

int Point2D::getY()
{
	return this->y;
}

void Point2D::setY(int y)
{
	this->y = y;
}

bool Point2D::operator==(const Point2D & point) const
{
	return this->x == point.x && this->y == point.y;
}

bool Point2D::operator!=(const Point2D & point) const
{
	return !(*this == point);
}

double Point2D::getDistanceFromPoint(Point2D * point)
{
	double xPow = pow(this->x - point->x, 2);
	double yPow = pow(this->y - point->y, 2);
	return sqrtl(xPow + yPow);
}

void Point2D::calcDistanceFromTarget(Point2D * targetPoint)
{
	this->h = getDistanceFromPoint(targetPoint);
}

void Point2D::set_g(int levelOfPointInTree)
{
	this->g = levelOfPointInTree;
}

double Point2D::get_g()
{
	return this->g;
}

double Point2D::get_h()
{
	return this->h;
}

void Point2D::set_f(Point2D* targetPoint, int levelOfPointInTree)
{
	set_g(levelOfPointInTree);
	calcDistanceFromTarget(targetPoint);
	this->f = this->g + this->h;
}

double Point2D::get_f()
{
	return this->f;
}

double Point2D::getDistanceFromTarget(Point2D * targetPoint)
{
	calcDistanceFromTarget(targetPoint);
	return this->get_h();
}
