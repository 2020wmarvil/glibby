#include "glibby/primitives/quad2D.h"
#include <cmath>
#include <vector>

namespace glibby
{
    Quad2D::Quad2D() {
        p1 = Point2();
        p2 = Point2();
        p3 = Point2();
        p4 = Point2();
    }
    Quad2D::Quad2D(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4) {
        p1 = Point2({ _x1, _y1 });
        p2 = Point2({ _x2, _y2 });
        p3 = Point2({ _x3, _y3 });
        p4 = Point2({ _x4, _y4 });
    }

    Quad2D::Quad2D(const Point2& _p1, const Point2& _p2, const Point2& _p3, const Point2& _p4) {
        p1 = Point2({ _p1.coord[0], _p1.coord[1] });
        p2 = Point2({ _p2.coord[0], _p2.coord[1] });
        p3 = Point2({ _p3.coord[0], _p3.coord[1] });
        p4 = Point2({ _p4.coord[0], _p4.coord[1] });
    }


    float Quad2D::area() const {
        float x1 = p1.coord[0], y1 = p1.coord[1];
        float x2 = p2.coord[0], y2 = p2.coord[1];
        float x3 = p3.coord[0], y3 = p3.coord[1];
        float x4 = p4.coord[0], y4 = p4.coord[1];

        //https://www.onlinemath4all.com/area-of-quadrilateral-when-four-vertices-are-given.html#:~:text=To%20find%20area%20of%20the,shown%20in%20the%20dark%20arrows.
        float area = 0.5 * abs((x1 * y2 + x2 * y3 + x3 * y4 + x4 * y1) - (y1 * x2 + y2 * x3 + y3 * x4 + y4 * x1));
        return area;
    }

    float Quad2D::perimeter() const {
        float a = distance(p1, p2);
        float b = distance(p2, p3);
        float c = distance(p3, p4);
        float d = distance(p4, p1);
        return a + b + c + d;
    }

    Point2 Quad2D::get_center() const {
        float cx = (p1.coord[0] + p2.coord[0] + p3.coord[0] + p4.coord[0]) / 4;
        float cy = (p1.coord[1] + p2.coord[1] + p3.coord[1] + p4.coord[1]) / 4;

        return Point2({ cx, cy });
    }

    bool Quad2D::intersects(const Quad2D& quad) {
        //Seperating axis theorem
        //https://gamedevelopment.tutsplus.com/tutorials/collision-detection-using-the-separating-axis-theorem--gamedev-169
        
        return false;
    }



    float Distance(const Quad2D& t1, const Quad2D& t2){
        Point2 c1 = t1.get_center();
        Point2 c2 = t2.get_center();
        return distance(c1, c2);
    }
    std::ostream& operator<<(std::ostream& out, const Quad2D& quad){
        out << "P1: " << quad.get_p1()
            << "\nP2: " << quad.get_p2()
            << "\nP3: " << quad.get_p3()
            << "\nP4: " << quad.get_p4();
        return out;
    }
}