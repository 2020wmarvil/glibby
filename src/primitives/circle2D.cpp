#define _USE_MATH_DEFINES

#include "glibby/primitives/circle2D.h"
#include "glibby/primitives/point.h"
#include <cmath>

namespace glibby
{
    Circle2D::Circle2D() {
        c = Point2();
        r = -1;
    }
    Circle2D::Circle2D(float _x, float _y, float _r) {
        c = Point2({ _x, _y });
        r = _r;
    }

    Circle2D::Circle2D(const Point2& _c, float _r) {
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
        float dist = distance(c, circle.get_center());
        return dist <= r + circle.get_radius();
    }

    //Circle x Triangle
    bool Circle2D::intersects(const Triangle2D& triangle) {
        Point2 center = c;
        float radius = r;
        Point2 p1 = triangle.get_p1();
        Point2 p2 = triangle.get_p2();
        Point2 p3 = triangle.get_p3();



        //http://www.phatcode.net/articles.php?id=459
        //Test 1: Triangle vertex in circle
        //Check if any of 3 points's distance from center is less than radius
        if (distance(center, p1) <= radius ||
            distance(center, p2) <= radius ||
            distance(center, p3) <= radius) {
            return true;
        }

        /*
        Test 2: Circle inside triangle
        https://i.imgur.com/YKkfvpu.png
        Validate center is in positive direction of circle (see image above).
        If all 3 sides are positive, then the triangle is inside of the circle
        */
        if ((p2.coord[1] - p1.coord[1]) * (center.coord[0] - p1.coord[0]) - 
                (p2.coord[0] - p1.coord[0]) * (center.coord[1] - p1.coord[1]) >= 0 && //P2 & P1
            (p3.coord[1] - p2.coord[1]) * (center.coord[0] - p2.coord[0]) -
                (p3.coord[0] - p2.coord[0]) * (center.coord[1] - p2.coord[1]) >= 0 && //P3 & P2
            (p1.coord[1] - p3.coord[1]) * (center.coord[0] - p3.coord[0]) -
                (p1.coord[0] - p3.coord[0]) * (center.coord[0] - p3.coord[0]) >= 0){ //P1 & P3
            
            return true;
        }

        /*
        Test 3: Circle intersects edge
        */

        //First edge
        float c1x = center.coord[0] - p1.coord[0];
        float c1y = center.coord[1] - p1.coord[1];
        float e1x = p2.coord[0] - p1.coord[0];
        float e1y = p2.coord[1] - p1.coord[1];

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
        float c2x = center.coord[0] - p2.coord[0];
        float c2y = center.coord[1] - p2.coord[1];
        float e2x = p3.coord[0] - p2.coord[0];
        float e2y = p3.coord[1] - p2.coord[1];

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
        float c3x = center.coord[0] - p3.coord[0];
        float c3y = center.coord[1] - p3.coord[1];
        float e3x = p1.coord[0] - p3.coord[0];
        float e3y = p1.coord[1] - p3.coord[1];

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