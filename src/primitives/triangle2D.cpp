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

    float Distance(const Triangle2D& t1, const Triangle2D& t2){
        //float deltaX = t1.x - t2.x;
		//float deltaY = t1.y - t2.y;
		//return std::sqrt(deltaX * deltaX + deltaY * deltaY);
        return -1;
    }
}