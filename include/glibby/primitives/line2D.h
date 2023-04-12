#pragma once
#pragma once
#include <iostream>
#include "glibby/primitives/point.h"

namespace glibby
{
    struct Line2D
    {
    public:
        //Constructors
        Line2D();
        Line2D(float _x1, float _y1, float _x2, float _y2);
        Line2D(const Point2& _p1, const Point2& _p2);

        //Getters
        Point2 get_p1() const { return p1; };
        Point2 get_p2() const { return p2; };

        float length() const;
        Point2 get_center() const;

        bool intersects(const Line2D& other);
    private:
        Point2 p1, p2;

        //Intersection functions
        bool areCollinearAndOverlapping(Point2 a1, Point2 b1, Point2 a2, Point2 b2);
    };

    float Distance(const Line2D& l1, const Line2D& l2);
    std::ostream& operator<< (std::ostream& out, const Line2D& line);

}