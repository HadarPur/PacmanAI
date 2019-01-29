#include "ComparePointsByDist.h"





bool ComparePointsByDist::operator()(Point2D* & p1, Point2D* & p2)
{
	return (p1->get_h() > p2->get_h());
}



