#include "glibby/glibby.h"

#include <math.h>

namespace glibby
{
	float Distance(const Point2D& p1, const Point2D& p2)
	{
		float deltaX = p1.x - p2.x;
		float deltaY = p1.y - p2.y;
		return sqrt(deltaX * deltaX + deltaY * deltaY);
	}
}