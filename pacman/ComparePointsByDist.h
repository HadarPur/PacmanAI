#pragma once
#ifndef __COMPAREPOINTSBYDIST_H
#define __COMPAREPOINTSBYDIST_H

#include "Point2D.h"

class ComparePointsByDist
{

public:

	bool operator() (Point2D* & p1, Point2D* & p2);
};

#endif // !__COMPAREPOINTSBYDIST_H