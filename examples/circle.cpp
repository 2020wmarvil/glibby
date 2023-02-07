#include "glibby/primitives/point2D.h"
#include "glibby/primitives/circle2D.h"

#include <iostream>

int main()
{
	glibby::Circle2D c1(Point2D(1, 1), 5);
	glibby::Circle2D c2(0, 0, 5);

	float distance = p1.distance(p2);
	std::cout << distance << std::endl;
}