#include "stdafx.h"
#include"Point.h"

void Point::operator+=(const Point& rtmp)
{
	x += rtmp.x;
	y += rtmp.y;
}

Point normalize(const Point& tmp) {
	float tmpl = Length(tmp);
	Point returntmp(tmp.x, tmp.y);

	if (tmpl != 0)
	{
		returntmp.x /= tmpl;
		returntmp.y /= tmpl;
	}
	return returntmp;
}

bool Collision(Point a, Point b, float range)
{
	if (Length(a - b) < range)
		return true;
	else
		return false;
}

float Length(const Point& tmp) {
	return	sqrtf(tmp.x*tmp.x + tmp.y*tmp.y);
}
