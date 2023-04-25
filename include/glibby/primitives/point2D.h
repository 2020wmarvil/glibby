#pragma once
#include <iostream>


namespace glibby
{
	struct Point2D
	{
		public:
			//Constructors
			Point2D();
			Point2D(float _x, float _y);

			//Getters
			float get_x() const {return x;}
			float get_y() const {return y;}
			float distance(const Point2D& p) const;

		private:
			float x;
			float y;
	};

	std::ostream& operator<< (std::ostream& out, const Point2D& point);
	
	//Computes the direction of the three given points
	int direction(Point2D p, Point2D q, Point2D r);
}
