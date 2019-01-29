#ifndef __POINT2D_H
#define __POINT2D_H
#include <math.h>

class Point2D
{
private:
	int x;
	int y;

	double g; // level of the point from the root (start point)
	double f; // function of the estimated cost depends on the distance of the point from start and target
	double h; // euclidian distance from Start point

public:
	Point2D(int x, int y);

	int getX();
	void setX(int x);
	int getY();
	void setY(int y);

	bool operator==(const Point2D& point) const;
	bool operator !=(const Point2D& point) const;

	double getDistanceFromPoint(Point2D* point);
	void calcDistanceFromTarget(Point2D* targetPoint);
	void set_g(int levelOfPointInTree);
	double get_g();
	double get_h();
	void set_f(Point2D* targetPoint, int levelOfPointInTree);
	double get_f();
	double getDistanceFromTarget(Point2D * targetPoint);

};

#endif // !__POINT2D_H