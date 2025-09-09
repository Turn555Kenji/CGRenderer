#include "point.h"
#include <QString>

Point::Point(double x, double y, int id, QString name, QString type)
    : Obj(id, name, type), Matrix(3, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 1;
}

//Used by lines and polygons
Point::Point(double x, double y)
    : Matrix(3, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 1;
}

Point::Point(Point *p )
    : Matrix(3, 1) {
    *this = p;
}

void draw(QPainter *painter) {

}

Obj* transform(Matrix m) {

}
