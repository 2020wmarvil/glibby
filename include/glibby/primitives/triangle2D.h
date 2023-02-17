#pragma once
#include "glibby/primitives/point2D.h"

namespace glibby
{
    struct Triangle2D
    {
    public:
        //Constructors
        Triangle2D();
        Triangle2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3);
        Triangle2D(const Point2D& _p1, const Point2D& _p2, const Point2D& _p3);

        //Getters
        float area() const;
        float perimeter() const;
        Point2D get_center() const;
    private:
        Point2D p1, p2, p3;
    };

    float Distance(const Triangle2D& t1, const Triangle2D& t2);


}