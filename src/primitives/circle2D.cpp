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
        Point2D center = c;
        float radius = r;
        Point2D p1 = triangle.get_p1();
        Point2D p2 = triangle.get_p2();
        Point2D p3 = triangle.get_p3();



        //http://www.phatcode.net/articles.php?id=459
        //Test 1: Triangle vertex in circle
        //Check if any of 3 points's distance from center is less than radius
        if (center.distance(p1) <= radius ||
            center.distance(p2) <= radius ||
            center.distance(p3) <= radius) {
            return true;
        }

        /*
        Test 2: Circle inside triangle
        https://i.imgur.com/YKkfvpu.png
        Validate center is in positive direction of circle (see image above).
        If all 3 sides are positive, then the triangle is inside of the circle
        */
        if ((p2.get_y() - p1.get_y()) * (center.get_x() - p1.get_x()) - 
                (p2.get_x() - p1.get_x()) * (center.get_y() - p1.get_y()) >= 0 && //P2 & P1
            (p3.get_y() - p2.get_y()) * (center.get_x() - p2.get_x()) -
                (p3.get_x() - p2.get_x()) * (center.get_y() - p2.get_y()) >= 0 && //P3 & P2
            (p1.get_y() - p3.get_y()) * (center.get_x() - p3.get_x()) -
                (p1.get_x() - p3.get_x()) * (center.get_x() - p3.get_x()) >= 0){ //P1 & P3
            
            return true;
        }

        /*
        Test 3: Circle intersects edge
        */

        //First edge
        float c1x = center.get_x() - p1.get_x();
        float c1y = center.get_y() - p1.get_y();
        float e1x = p2.get_x() - p1.get_x();
        float e1y = p2.get_y() - p1.get_y();

        float k = c1x * e1x + c1y * e1y;
        if (k > 0) {
            float len = sqrt(e1x * e1x + e1y * e1y);
            k = k / len;

            if (k < len) {
                if (sqrt(c1x * c1x + c1y * c1y - k * k) <= radius) {
                    return true;
                }
            }
        }

        //Second edge
        float c2x = center.get_x() - p2.get_x();
        float c2y = center.get_y() - p2.get_y();
        float e2x = p3.get_x() - p2.get_x();
        float e2y = p3.get_y() - p2.get_y();

        k = c2x * e2x + c2y * e2y;
        if (k > 0) {
            float len = sqrt(e2x * e2x + e2y * e2y);
            k = k / len;

            if (k < len) {
                if (sqrt(c2x * c2x + c2y * c2y - k * k) <= radius) {
                    return true;
                }
            }
        }

        //Third edge
        float c3x = center.get_x() - p3.get_x();
        float c3y = center.get_y() - p3.get_y();
        float e3x = p1.get_x() - p3.get_x();
        float e3y = p1.get_y() - p3.get_y();

        k = c3x * e3x + c3y * e3y;
        if (k > 0) {
            float len = sqrt(e3x * e3x + e3y * e3y);
            k = k / len;

            if (k < len) {
                if (sqrt(c3x * c3x + c3y * c3y - k * k) <= radius) {
                    return true;
                }
            }
        }


        return false;
    }

    //Output
    std::ostream& operator<< (std::ostream& out, const Circle2D& circle) {
        out << circle.get_center() << " : r = " << circle.get_radius();
        return out;
    }
}