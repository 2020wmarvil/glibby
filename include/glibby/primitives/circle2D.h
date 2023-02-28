#pragma once
#include <iostream>
#include "glibby/primitives/point2D.h"
#include "glibby/primitives/triangle2D.h"


namespace glibby
{
    struct Circle2D
    {
    public:
        //Constructors
        Circle2D();
        Circle2D(float _x, float _y, float _r);
        Circle2D(const Point2D& _c, float _r);
        Circle2D(const Circle2D& other);

        //Accessors
        const Point2D& get_center() const {return c;}
        float get_radius() const {return r;}

        float area() const;
        float perimeter() const;

        bool intersects(const Circle2D& circle);
        bool intersects(const Triangle2D& triangle);

        //http://www.phatcode.net/articles.php?id=459

    private:
        //Member Variables
        Point2D c;
        float r;
    };

    std::ostream& operator<< (std::ostream& out, const Circle2D& circle);
}