#include "glibby/primitives/triangle2D.h"
#include <cmath>

namespace glibby
{
    Triangle2D::Triangle2D() {
        p1 = Point2();
        p2 = Point2();
        p3 = Point2();
    }
    Triangle2D::Triangle2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3) {
        p1 = Point2({ _x1, _y1 });
        p2 = Point2({ _x2, _y2 });
        p3 = Point2({ _x3, _y3 });

    }

    Triangle2D::Triangle2D(const Point2& _p1, const Point2& _p2, const Point2& _p3) {
        p1 = Point2({ _p1.coord[0], _p1.coord[1] });
        p2 = Point2({ _p2.coord[0], _p2.coord[1] });
        p3 = Point2({ _p3.coord[0], _p3.coord[1] });
    }


    float Triangle2D::area() const {
        float a = distance(p1, p2);
        float b = distance(p2, p3);
        float c = distance(p3, p1);
        float s = (a + b + c) / 2; //Heron's formula
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    float Triangle2D::perimeter() const {
        float a = distance(p1, p2);
        float b = distance(p2, p3);
        float c = distance(p3, p1);
        return a + b + c;
    }

    Point2 Triangle2D::get_center() const {
        float cx = (p1.coord[0] + p2.coord[0] + p3.coord[0])/3;
        float cy = (p1.coord[1] + p2.coord[1] + p3.coord[1])/3;

        return Point2({ cx, cy });
    }

    float Distance(const Triangle2D& t1, const Triangle2D& t2){
        Point2 c1 = t1.get_center();
        Point2 c2 = t2.get_center();
        return distance(c1, c2);
    }
    std::ostream& operator<<(std::ostream& out, const Triangle2D& triangle){
        out << "P1: " << triangle.get_p1()
            << "\nP2: " << triangle.get_p2()
            << "\nP3: " << triangle.get_p3();
        return out;
    }
}