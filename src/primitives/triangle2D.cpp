#include "glibby/primitives/triangle2D.h"
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
        p1 = p1;
        p2 = p2;
        p3 = p3;
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
    std::ostream& operator<<(std::ostream& out, const Triangle2D& triangle){
        out << "P1: " << triangle.get_p1()
            << "\nP2: " << triangle.get_p2()
            << "\nP3: " << triangle.get_p3();
        return out;
    }
}