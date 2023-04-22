#include "glibby/primitives/point.h"

#include <iostream>

int main()
{
	glibby::Point2 p1({ 0.0f, 0.0f });
	glibby::Point2 p2({ 3.0f, 2.0f });

	float distance = glibby::EuclideanDistance(p1, p2);
	std::cout << distance << std::endl;
}