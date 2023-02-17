#include "glibby/primitives/point2D.h"
#include "glibby/primitives/circle2D.h"

#include <iostream>

int main()
{
	glibby::Circle2D c1(glibby::Point2D(1, 1), 5);
	glibby::Circle2D c2(0, 0, 1);

	float distance = c1.get_center().distance(c2.get_center());
	std::cout << "Circle 1: " << c1 << std::endl;
	std::cout << "Circle 2: " << c2 << std::endl;
	std::cout << "Distance: " << distance << std::endl;
	std::cout << "Area: " << c2.area() << std::endl;
	std::cout << "Perimeter: " << c2.perimeter() << std::endl;
	std::cout << std::boolalpha << "Intersect: " << c1.intersects(c2) << std::endl;



}