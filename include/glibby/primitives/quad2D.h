#pragma once
#pragma once
#include <iostream>
#include "glibby/primitives/point.h"

namespace glibby
{
    struct Quad2D
    {
    public:
        //Constructors
        Quad2D();
        Quad2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4);
        Quad2D(const Point2& _p1, const Point2& _p2, const Point2& _p3, const Point2& _p4);

        //Getters
        Point2 get_p1() const { return p1; };
        Point2 get_p2() const { return p2; };
        Point2 get_p3() const { return p3; };
        Point2 get_p4() const { return p4; };

        float area() const;
        float perimeter() const;
        Point2 get_center() const;

        bool intersects(const Quad2D& quad);
    private:
        Point2 p1, p2, p3, p4;
    };

    float Distance(const Quad2D& r1, const Quad2D& r2);
    std::ostream& operator<< (std::ostream& out, const Quad2D& quad);

}