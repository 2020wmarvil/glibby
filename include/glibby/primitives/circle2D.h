#pragma once

namespace glibby
{
    struct Circle2D
    {
        float area() const;
        float perimeter() const;

        float x;
        float y;
        float r;
    };

    float Distance(const Circle2D& c1, const Circle2D& c2);


}