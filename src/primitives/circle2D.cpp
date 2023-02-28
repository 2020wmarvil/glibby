#define _USE_MATH_DEFINES

#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/point2D.h"
#include <cmath>

namespace glibby
{
    Circle2D::Circle2D() {
        c = Point2D();
        r = -1;
    }
    Circle2D::Circle2D(float _x, float _y, float _r) {
        c = Point2D(_x, _y);
        r = _r;
    }

    Circle2D::Circle2D(const Point2D& _c, float _r) {
        c = _c;
        r = _r;
    }

    Circle2D::Circle2D(const Circle2D& other) {
        c = other.c;
        r = other.r;
    }



    float Circle2D::area() const {
        return float(M_PI * r * r);
    }

    float Circle2D::perimeter() const {
        return 2 * M_PI * r;
    }

    //Circle x Circle
    bool Circle2D::intersects(const Circle2D& circle) {
        float dist = c.distance(circle.get_center());
        return dist <= r + circle.get_radius();
    }

    //Circle x Triangle
    bool Circle2D::intersects(const Triangle2D& triangle) {
        Circle2D circle(*this);

        //http://www.phatcode.net/articles.php?id=459
        //Test 1: Triangle vertex in circle
        //Check if any of 3 points's distance from center is less than radius
        if (circle.get_center().distance(triangle.get_p1()) <= circle.get_radius() ||
            circle.get_center().distance(triangle.get_p2()) <= circle.get_radius() ||
            circle.get_center().distance(triangle.get_p3()) <= circle.get_radius()) {
            return true;
        }

        /*
        Test 2: Circle inside triangle
        https://i.imgur.com/YKkfvpu.png
        Validate center is in positive direction of circle (see image above).
        If all 3 sides are positive, then the triangle is inside of the circle
        */
        if ((triangle.get_p2().get_y() - triangle.get_p1().get_y()) * (circle.get_center().get_x() - triangle.get_p1().get_x()) - 
                (triangle.get_p2().get_x() - triangle.get_p1().get_x()) * (circle.get_center().get_y() - triangle.get_p1().get_y()) >= 0 && //P2 & P1
            (triangle.get_p3().get_y() - triangle.get_p2().get_y()) * (circle.get_center().get_x() - triangle.get_p2().get_x()) -
                (triangle.get_p3().get_x() - triangle.get_p2().get_x()) * (circle.get_center().get_y() - triangle.get_p2().get_y()) >= 0 && //P3 & P2
            (triangle.get_p1().get_y() - triangle.get_p3().get_y()) * (circle.get_center().get_x() - triangle.get_p3().get_x()) -
                (triangle.get_p1().get_x() - triangle.get_p3().get_x()) * (circle.get_center().get_x() - triangle.get_p3().get_x()) >= 0){ //P1 & P3
            
            return true;
        }
        return false;
    }

    //Output
    std::ostream& operator<< (std::ostream& out, const Circle2D& circle) {
        out << circle.get_center() << " : r = " << circle.get_radius();
        return out;
    }
}