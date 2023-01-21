#include "glibby/primitives/point2D.h"

#include <iostream>

int main()
{
	glibby::Point2D p1 = { 0.0f, 0.0f };
	glibby::Point2D p2 = { 3.0f, 2.0f };

	float distance = glibby::Distance(p1, p2);
	std::cout << distance << std::endl;
}