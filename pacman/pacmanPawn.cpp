#include "pacmanPawn.h"



PacmanPawn::PacmanPawn(Point2D* startPoint, int scale) 
{
	this->translationPoint = new Point2D(startPoint->getX(), startPoint->getY());
	this->scale = scale;
}


PacmanPawn::~PacmanPawn()
{
	if (this->translationPoint)
		delete this->translationPoint;
}

void PacmanPawn::drawPacmanClosed()
{
	glPushMatrix();
	glTranslated(this->translationPoint->getX(), this->translationPoint->getY(), 0);
	glRotated(this->rotationAngle, 0, 0, 1);
	glScaled(this->scale, this->scale, 1);

	double PI = 3.14;
	double alpha, x, y, radius = 1, delta = PI / 20;

	//draw the outline of the wheel
	glColor3d(1, 1, 0);
	glBegin(GL_POLYGON);
	for (alpha = 0; alpha <= 2 * PI; alpha += delta)
	{

		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex2d(x, y);


	}
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2d(0, 0);
	glVertex2d(radius, 0);
	glEnd();

	glPopMatrix();

	
}

void PacmanPawn::drawPacmanOpen()
{
	glPushMatrix();
	
	glTranslated(this->translationPoint->getX(), this->translationPoint->getY(), 0);
	glRotated(this->rotationAngle, 0, 0, 1);
	glScaled(this->scale, this->scale, 1);
	


	double PI = 3.14;
	double alpha, x, y, radius = 1, delta = PI / 20;

	//draw the outline of the wheel
	glColor3d(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	for (alpha = (3*delta); alpha <= (2 * PI) - (3*delta); alpha += delta)
	{

		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex2d(x, y);
	}
	glVertex2d(0, 0);
	glEnd();

	glPopMatrix();
}

void PacmanPawn::setIsOpen(bool isOpen)
{
	this->isOpen = isOpen;
}

void PacmanPawn::changeIsOpen()
{
	this->isOpen = !this->isOpen;
}

void PacmanPawn::drawPacman()
{
	if (this->isOpen)
		drawPacmanOpen();
	else drawPacmanClosed();
}

Point2D * PacmanPawn::getPacmanLocation()
{
	return this->translationPoint;
}

void PacmanPawn::setTranslation(PacmanPawn::pacmanDirection dir, Point2D * toPoint)
{
	switch (dir)
	{
	case PacmanPawn::Up:
		this->rotationAngle = 90;
		break;
	case PacmanPawn::Down:
		this->rotationAngle = -90;
		break;
	case PacmanPawn::Left:
		this->rotationAngle = 180;
		break;
	case PacmanPawn::Right:
		this->rotationAngle = 0;
		break;
	default:
		break;
	}
	this->translationPoint->setX(toPoint->getX());
	this->translationPoint->setY(toPoint->getY());
}


int PacmanPawn::getScale()
{
	return this->scale;
}

PacmanPawn::pacmanDirection PacmanPawn::getDirection()
{
	switch (this->rotationAngle)
	{
	case 0:
		return PacmanPawn::pacmanDirection::Right;
		break;
	case 90:
		return PacmanPawn::pacmanDirection::Up;
		break;
	case -90:
		return PacmanPawn::pacmanDirection::Down;
		break;
	case 180:
		return PacmanPawn::pacmanDirection::Left;
		break;
	}
}
