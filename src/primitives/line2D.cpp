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
        //Get points
        float x1 = p1.get_x(), y1 = p1.get_y();
        float x2 = p2.get_x(), y2 = p2.get_y();

        //Distance in each disrection
        float dx = x2 - x1;
        float dy = y2 - y1;

        //Length
        float len = sqrt(dx * dx + dy * dy);
        return len;
    }

    Point2D Line2D::get_center() const {
        float cx = (p1.get_x() + p2.get_x()) / 2;
        float cy = (p1.get_y() + p2.get_y()) / 2;

        return Point2D(cx, cy);
    }

    //https://www.codingninjas.com/codestudio/library/check-if-two-line-segments-intersect#:~:text=If%20the%20orientations%20of%20the,line%20segments%20do%20not%20intersect.
    bool Line2D::intersects(const Line2D& line) {
        Point2D a1 = p1;
        Point2D b1 = p2;
        Point2D a2 = line.get_p1();
        Point2D b2 = line.get_p2();

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
    bool Line2D::areCollinearAndOverlapping(Point2D a1, Point2D b1, Point2D a2, Point2D b2) {
        // Check if the line segments are collinear
        if (direction(a1, b1, a2) == 0) {
            // Check if the line segments overlap
            if (a2.get_x() <= std::max(a1.get_x(), b1.get_x()) && 
                a2.get_x() >= std::min(a1.get_x(), b1.get_x()) && 
                a2.get_y() <= std::max(a1.get_y(), b1.get_y()) && 
                a2.get_y() >= std::min(a1.get_y(), b1.get_y())
                ) {
                return true;
            }
        }
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