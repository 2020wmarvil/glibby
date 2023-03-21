#include "glibby/primitives/point2D.h"
#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/triangle2D.h"
#include "glibby/primitives/quad2D.h"

#include <iostream>
#include <assert.h>

int main()
{
	glibby::Circle2D c1(glibby::Point2D(1, 1), 1);
	glibby::Circle2D c2(0, 0, 5);

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
	glibby::Triangle2D t1(glibby::Triangle2D(glibby::Point2D(2, 2), glibby::Point2D(10, 4), glibby::Point2D(5, 7)));
	glibby::Triangle2D t2(5, 0, 10, 4, 5, 7);
	glibby::Triangle2D t3(glibby::Triangle2D(glibby::Point2D(5, 1), glibby::Point2D(5, -1), glibby::Point2D(10, 0)));
	glibby::Triangle2D t4(glibby::Triangle2D(glibby::Point2D(0, 1), glibby::Point2D(-1, -1), glibby::Point2D(1, -1)));
	glibby::Triangle2D t5(glibby::Triangle2D(glibby::Point2D(0, 5), glibby::Point2D(-5, -5), glibby::Point2D(5, -5)));
	glibby::Triangle2D out_triangle(50, 50, 100, 50, 50, 100);

	std::cout << "Triangle 1: \n" << t1 << std::endl;
	std::cout << "Triangle 2: \n" << t2 << std::endl;

	/*Circle x Triangle*/
	//https://i.imgur.com/dDMdPOR.png
	std::cout << "C2 & T1 intersect: " << c2.intersects(t1) << std::endl;
	assert(c2.intersects(t1) == true);
	//https://i.imgur.com/pebtxsh.png
	std::cout << "C2 & T2 intersect: " << c2.intersects(t2) << std::endl;
	assert(c2.intersects(t2) == true);
	//https://i.imgur.com/pLBK527.png
	std::cout << "C2 & T3 intersect: " << c2.intersects(t3) << std::endl;
	assert(c2.intersects(t3) == true);
	//https://i.imgur.com/1LN0na1.png
	std::cout << "C2 & T4 intersect: " << c2.intersects(t4) << std::endl;
	assert(c2.intersects(t4) == true);
	//https://i.imgur.com/GrnUwR6.png
	std::cout << "C1 & T5 intersect: " << c1.intersects(t5) << std::endl;
	assert(c1.intersects(t5) == true);

	//Out of bounds
	std::cout << "C2 & Out Triangle intersect: " << c2.intersects(out_triangle) << std::endl;
	assert(c2.intersects(out_triangle) == false);

	/*QUADRILATERALS*/
	glibby::Quad2D q1(glibby::Quad2D(glibby::Point2D(0, 0), glibby::Point2D(0, 5), glibby::Point2D(5, 5), glibby::Point2D(5, 0)));
	std::cout << "Quadrilateral 1: \n" << q1 << std::endl;
	assert(q1.area() == 25);
	assert(q1.get_center().get_x() == 2.5 && q1.get_center().get_y() == 2.5);
	assert(q1.perimeter() == 20);

}