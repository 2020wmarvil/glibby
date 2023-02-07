#include "glibby/primitives/point2D.h"

#include <cmath>

namespace glibby
{
	//Constructors
	Point2D::Point2D(){
		x = -1;
		y = -1;
	}

	Point2D::Point2D(float _x, float _y) {
		x = _x;
		y = _y;
	}

	float Point2D::distance(const Point2D& p) const {
		float deltaX = x - p.x;
		float deltaY = y - p.y;
		return std::sqrt(deltaX * deltaX + deltaY * deltaY);
	}
}
