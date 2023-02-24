#define _USE_MATH_DEFINES

#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/point2D.h"
#include <cmath>

namespace glibby
{
    Circle2D::Circle2D(){
        c = Point2D();
        r = -1;
    }
    Circle2D::Circle2D(float _x, float _y, float _r){
        c = Point2D(_x,_y);
        r = _r;
    }

    Circle2D::Circle2D(const Point2D& _c, float _r){
        c = _c;
        r = _r;
    }


    float Circle2D::area() const {
        return M_PI * r * r;
    }

    float Circle2D::perimeter() const {
        return 2 * M_PI * r;
    }

    bool Circle2D::intersects(Circle2D circle) {
        float dist = c.distance(circle.get_center());
        return dist <= r + circle.get_radius();
    }

    //Output
    std::ostream& operator<< (std::ostream& out, const Circle2D& circle) {
        out << circle.get_center() << " : r = " << circle.get_radius();
        return out;
    }
}