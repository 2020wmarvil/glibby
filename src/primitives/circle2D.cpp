#define _USE_MATH_DEFINES

#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/point2D.h"
#include <cmath>

namespace glibby
{
    Circle2D::Circle2D(float x, float y, float _r){
        Point2D p(x, y);
        c = p;
        r = _r;
    }

    float Circle2D::area() const {
        return M_PI * r * r;
    }

    float Circle2D::perimeter() const {
        return 2 * M_PI * r;
    }
}