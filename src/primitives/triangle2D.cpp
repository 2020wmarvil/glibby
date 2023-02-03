#include "glibby/primitives/triangle2D.h"
#include <cmath>

namespace glibby
{
    float Triangle2D::area() const {
        return -1;
    }

    float Triangle2D::perimeter() const {
        return -1;
    }

    float Distance(const Triangle2D& t1, const Triangle2D& t2){
        float deltaX = t1.x - t2.x;
		float deltaY = t1.y - t2.y;
		return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }
}