#include "glibby/primitives/triangle2D.h"
#include "glibby/primitives/circle2D.h"

#include <cmath>

namespace glibby
{
    Triangle2D::Triangle2D() {
        p1 = Point2D();
        p2 = Point2D();
        p3 = Point2D();
    }
    Triangle2D::Triangle2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3) {
        p1 = Point2D(_x1, _y1);
        p2 = Point2D(_x2, _y2);
        p3 = Point2D(_x3, _y3);

    }

    Triangle2D::Triangle2D(const Point2D& _p1, const Point2D& _p2, const Point2D& _p3) {
        p1 = Point2D(_p1.get_x(), _p1.get_y());
        p2 = Point2D(_p2.get_x(), _p2.get_y());
        p3 = Point2D(_p3.get_x(), _p3.get_y());
    }


    float Triangle2D::area() const {
        float a = p1.distance(p2);
        float b = p2.distance(p3);
        float c = p3.distance(p1);
        float s = (a + b + c) / 2; //Heron's formula
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    float Triangle2D::perimeter() const {
        float a = p1.distance(p2);
        float b = p2.distance(p3);
        float c = p3.distance(p1);
        return a + b + c;
    }

    Point2D Triangle2D::get_center() const {
        float cx = (p1.get_x() + p2.get_x() + p3.get_x())/3;
        float cy = (p1.get_y() + p2.get_y() + p3.get_y())/3;

        return Point2D(cx, cy);
    }

    float Distance(const Triangle2D& t1, const Triangle2D& t2){
        Point2D c1 = t1.get_center();
        Point2D c2 = t2.get_center();
        return c1.distance(c2);
    }

    //Circle x Triangle
    bool Triangle2D::intersects(const Circle2D& circle) {
        Point2D center = circle.get_center();
        float radius = circle.get_radius();
        Point2D p1 = p1;
        Point2D p2 = p2;
        Point2D p3 = p3;



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
            (p1.get_x() - p3.get_x()) * (center.get_x() - p3.get_x()) >= 0) { //P1 & P3

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

    std::ostream& operator<<(std::ostream& out, const Triangle2D& triangle){
        out << "P1: " << triangle.get_p1()
            << "\nP2: " << triangle.get_p2()
            << "\nP3: " << triangle.get_p3();
        return out;
    }
}