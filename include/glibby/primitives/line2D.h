#pragma once
#pragma once
#include <iostream>
#include "glibby/primitives/point2D.h"

namespace glibby
{
    struct Line2D
    {
    public:
        //Constructors
        Line2D();
        Line2D(float _x1, float _y1, float _x2, float _y2);
        Line2D(const Point2D& _p1, const Point2D& _p2);

        //Getters
        Point2D get_p1() const { return p1; };
        Point2D get_p2() const { return p2; };

        float length() const;
        Point2D get_center() const;

        bool intersects(const Line2D& other);
    private:
        Point2D p1, p2;

        //Intersection functions
        bool Line2D::areCollinearAndOverlapping(Point2D a1, Point2D b1, Point2D a2, Point2D b2);
    };

    float Distance(const Line2D& l1, const Line2D& l2);
    std::ostream& operator<< (std::ostream& out, const Line2D& line);

}