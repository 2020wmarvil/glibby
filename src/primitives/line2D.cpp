#include "glibby/primitives/line2D.h"
#include <cmath>
#include <vector>

namespace glibby
{
    Line2D::Line2D() {
        p1 = Point2();
        p2 = Point2();
    }
    Line2D::Line2D(float _x1, float _y1, float _x2, float _y2) {
        p1 = Point2({ _x1, _y1 });
        p2 = Point2({ _x2, _y2 });
    }

    Line2D::Line2D(const Point2& _p1, const Point2& _p2) {
        p1 = Point2({ _p1.coord[0], _p1.coord[1] });
        p2 = Point2({ _p2.coord[0], _p2.coord[1] });
    }


    float Line2D::length() const {
        //Get points
        float x1 = p1.coord[0], y1 = p1.coord[1];
        float x2 = p2.coord[0], y2 = p2.coord[1];

        //Distance in each disrection
        float dx = x2 - x1;
        float dy = y2 - y1;

        //Length
        float len = sqrt(dx * dx + dy * dy);
        return len;
    }

    Point2 Line2D::get_center() const {
        float cx = (p1.coord[0] + p2.coord[0]) / 2;
        float cy = (p1.coord[1] + p2.coord[1]) / 2;

        return Point2({ cx, cy });
    }

    //https://www.codingninjas.com/codestudio/library/check-if-two-line-segments-intersect#:~:text=If%20the%20orientations%20of%20the,line%20segments%20do%20not%20intersect.
    bool Line2D::intersects(const Line2D& line) {
        Point2 a1 = p1;
        Point2 b1 = p2;
        Point2 a2 = line.get_p1();
        Point2 b2 = line.get_p2();

        // Compute the directions of the four line segments
        int d1 = direction(a1, b1, a2);
        int d2 = direction(a1, b1, b2);
        int d3 = direction(a2, b2, a1);
        int d4 = direction(a2, b2, b1);

        // Check if the two line segments intersect
        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
            return true;
        }

        // Check if the line segments are collinear and overlapping
        if (areCollinearAndOverlapping(a1, b1, a2, b2) || areCollinearAndOverlapping(a2, b2, a1, b1)) {
            return true;
        }

        return false;
    }

    // Checks if two line segments are collinear and overlapping
    bool Line2D::areCollinearAndOverlapping(Point2 a1, Point2 b1, Point2 a2, Point2 b2) {
        // Check if the line segments are collinear
        if (direction(a1, b1, a2) == 0) {
            // Check if the line segments overlap
            if (a2.coord[0] <= std::max(a1.coord[0], b1.coord[0]) && 
                a2.coord[0] >= std::min(a1.coord[0], b1.coord[0]) && 
                a2.coord[1] <= std::max(a1.coord[1], b1.coord[1]) && 
                a2.coord[1] >= std::min(a1.coord[1], b1.coord[1])
                ) {
                return true;
            }
        }
        return false;
    }

    

    float Distance(const Line2D& l1, const Line2D& l2) {
        Point2 c1 = l1.get_center();
        Point2 c2 = l2.get_center();
        return distance(c1, c2);
    }
    std::ostream& operator<<(std::ostream& out, const Line2D& line) {
        out << "P1: " << line.get_p1()
            << "\nP2: " << line.get_p2();
        return out;
    }

    
}