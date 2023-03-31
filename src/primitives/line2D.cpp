#include "glibby/primitives/line2D.h"
#include <cmath>
#include <vector>

namespace glibby
{
    Line2D::Line2D() {
        p1 = Point2D();
        p2 = Point2D();
    }
    Line2D::Line2D(float _x1, float _y1, float _x2, float _y2) {
        p1 = Point2D(_x1, _y1);
        p2 = Point2D(_x2, _y2);
    }

    Line2D::Line2D(const Point2D& _p1, const Point2D& _p2) {
        p1 = Point2D(_p1.get_x(), _p1.get_y());
        p2 = Point2D(_p2.get_x(), _p2.get_y());
    }


    float Line2D::length() const {
        float x1 = p1.get_x(), y1 = p1.get_y();
        float x2 = p2.get_x(), y2 = p2.get_y();

        //Finish

        return -1;
    }

    Point2D Line2D::get_center() const {
        float cx = (p1.get_x() + p2.get_x()) / 2;
        float cy = (p1.get_y() + p2.get_y()) / 2;

        return Point2D(cx, cy);
    }

    bool Line2D::intersects(const Line2D& line) {
        

        return false;
    }



    float Distance(const Line2D& l1, const Line2D& l2) {
        Point2D c1 = l1.get_center();
        Point2D c2 = l2.get_center();
        return c1.distance(c2);
    }
    std::ostream& operator<<(std::ostream& out, const Line2D& line) {
        out << "P1: " << line.get_p1()
            << "\nP2: " << line.get_p2();
        return out;
    }
}