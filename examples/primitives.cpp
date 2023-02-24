#include "glibby/primitives/point2D.h"
#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/triangle2D.h"

#include <iostream>

int main()
{
	glibby::Circle2D c1(glibby::Point2D(1, 1), 5);
	glibby::Circle2D c2(0, 0, 1);

	/*CIRCLES*/
	//Print circle
	std::cout << "Circle 1: " << c1 << std::endl;
	std::cout << "Circle 2: " << c2 << std::endl;

	//Find distance
	float distance = c1.get_center().distance(c2.get_center());
	std::cout << "Distance: " << distance << std::endl;

	//Area / Perimeter
	std::cout << "Area: " << c2.area() << std::endl;
	std::cout << "Perimeter: " << c2.perimeter() << std::endl;
	
	//Intersect
	std::cout << std::boolalpha << "Intersect: " << c1.intersects(c2) << std::endl;

	/*TRIANGLES*/
	glibby::Triangle2D t1(glibby::Triangle2D(glibby::Point2D(1, 0), glibby::Point2D(0, 0), glibby::Point2D(0, 1)));
	glibby::Triangle2D t2(2, 2, 3, 3, 2, 3);

	std::cout << "Triangle 1: \n" << t1 << std::endl;
	std::cout << "Triangle 2: \n" << t2 << std::endl;
}