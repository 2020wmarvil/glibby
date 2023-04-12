#pragma once
#include <iostream>
#include "glibby/primitives/point.h"

namespace glibby
{
    struct Triangle2D
    {
    public:
        //Constructors
        Triangle2D();
        Triangle2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3);
        Triangle2D(const Point2& _p1, const Point2& _p2, const Point2& _p3);

        //Getters
        Point2 get_p1() const { return p1; };
        Point2 get_p2() const { return p2; };
        Point2 get_p3() const { return p3; };

        float area() const;
        float perimeter() const;
        Point2 get_center() const;
    private:
        Point2 p1, p2, p3;
    };

    float Distance(const Triangle2D& t1, const Triangle2D& t2);
    std::ostream& operator<< (std::ostream& out, const Triangle2D& triangle);

}