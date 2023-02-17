#pragma once
#include "glibby/primitives/point2D.h"


namespace glibby
{
    struct Circle2D
    {
    public:
        //Constructors
        Circle2D();
        Circle2D(float _x, float _y, float _r);
        Circle2D(const Point2D& _c, float _r);

        //Accessors
        const Point2D& get_center() const {return c;}
        float get_radius() const {return r;}

        float area() const;
        float perimeter() const;

    private:
        //Member Variables
        Point2D c;
        float r;
    };

}