#include "Monster.h"



Monster::Monster(vector<double> color, Point2D* startPoint, double scale)
{
	this->translationPoint = new Point2D(startPoint->getX(), startPoint->getY());
	this->scale = scale;
	this->color = color;
}


Monster::~Monster()
{
	if (this->translationPoint)
		delete this->translationPoint;
}

void Monster::drawMonster()
{
	double x, y;
	glPushMatrix();
	glTranslated(this->translationPoint->getX(), this->translationPoint->getY(), 0);
	glScaled(this->scale, this->scale, 1);
	glColor3d(this->color[0], this->color[1], this->color[2]);
	glBegin(GL_POLYGON);

	for (x = -1.0; x <= 1.0; x += 0.05)
	{
		y = -2*x*x + 1;
		glVertex2d(x, y);
	}

	glEnd();
	glPopMatrix();

}

void Monster::setTranslation(Point2D * toPoint)
{
	this->translationPoint->setX(toPoint->getX());
	this->translationPoint->setY(toPoint->getY());
}

Point2D * Monster::getLocation()
{
	return this->translationPoint;
}
