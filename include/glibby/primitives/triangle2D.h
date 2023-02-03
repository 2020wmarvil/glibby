#pragma once

namespace glibby
{
    struct Triangle2D
    {
        float area() const;
        float perimeter() const;

        float x;
        float y;
        float r;
    };

    float Distance(const Triangle2D& t1, const Triangle2D& t2);


}