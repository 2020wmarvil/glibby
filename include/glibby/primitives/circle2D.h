#pragma once
#include "glibby/primitives/point2D.h"


namespace glibby
{
    struct Circle2D
    {
        //Constructors
        Circle2D(float x, float y, float _r);
        Circle2D(Point2D c, float r);

        //Accessors
        float area() const;
        float perimeter() const;

        //Member Variables
        Point2D c;
        float r;
    };

}