#include "glibby/primitives/quad2D.h"
#include <cmath>
#include <vector>

namespace glibby
{
    Quad2D::Quad2D() {
        p1 = Point2D();
        p2 = Point2D();
        p3 = Point2D();
    }
    Quad2D::Quad2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4) {
        p1 = Point2D(_x1, _y1);
        p2 = Point2D(_x2, _y2);
        p3 = Point2D(_x3, _y3);
        p4 = Point2D(_x4, _y4);
    }

    Quad2D::Quad2D(const Point2D& _p1, const Point2D& _p2, const Point2D& _p3, const Point2D& _p4) {
        p1 = Point2D(_p1.get_x(), _p1.get_y());
        p2 = Point2D(_p2.get_x(), _p2.get_y());
        p3 = Point2D(_p3.get_x(), _p3.get_y());
        p4 = Point2D(_p4.get_x(), _p4.get_y());
    }


    float Quad2D::area() const {
        float x1 = p1.get_x(), y1 = p1.get_y();
        float x2 = p2.get_x(), y2 = p2.get_y();
        float x3 = p3.get_x(), y3 = p3.get_y();
        float x4 = p4.get_x(), y4 = p4.get_y();

        //https://www.onlinemath4all.com/area-of-quadrilateral-when-four-vertices-are-given.html#:~:text=To%20find%20area%20of%20the,shown%20in%20the%20dark%20arrows.
        float area = 0.5 * abs((x1 * y2 + x2 * y3 + x3 * y4 + x4 * y1) - (y1 * x2 + y2 * x3 + y3 * x4 + y4 * x1));
        return area;
    }

    float Quad2D::perimeter() const {
        float a = p1.distance(p2);
        float b = p2.distance(p3);
        float c = p3.distance(p4);
        float d = p4.distance(p1);
        return a + b + c + d;
    }

    Point2D Quad2D::get_center() const {
        float cx = (p1.get_x() + p2.get_x() + p3.get_x() + p4.get_x()) / 4;
        float cy = (p1.get_y() + p2.get_y() + p3.get_y() + p4.get_y()) / 4;

        return Point2D(cx, cy);
    }

    bool Quad2D::intersects(const Quad2D& quad) {
        //Seperating axis theorem
        
    }



    float Distance(const Quad2D& t1, const Quad2D& t2){
        Point2D c1 = t1.get_center();
        Point2D c2 = t2.get_center();
        return c1.distance(c2);
    }
    std::ostream& operator<<(std::ostream& out, const Quad2D& quad){
        out << "P1: " << quad.get_p1()
            << "\nP2: " << quad.get_p2()
            << "\nP3: " << quad.get_p3()
            << "\nP4: " << quad.get_p4();
        return out;
    }
}