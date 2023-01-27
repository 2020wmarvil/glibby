#define _USE_MATH_DEFINES

#include "glibby/primitives/circle2D.h"
#include <cmath>

namespace glibby
{
    float Circle2D::area() const {
        return M_PI * r * r;
    }

    float Circle2D::perimeter() const {
        return 2 * M_PI * r;
    }

    float Distance(const Circle2D& c1, const Circle2D& c2){
        float deltaX = c1.x - c2.x;
		float deltaY = c1.y - c2.y;
		return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }
}