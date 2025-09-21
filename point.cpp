#include "point.h"
#include <QString>

Point::Point(double x, double y, int id, QString name)
    : Obj(id, name, Type::Point), Matrix(3, 1) {
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

void Point::draw(QPainter *painter) {

    QPen pen (Qt ::blue, 5);
    pen.setCapStyle(Qt::RoundCap); // Crucial para o ponto parecer redondo
    painter->setPen(pen);
    painter->drawPoint(static_cast<int>((*this)[0][0]), static_cast<int>((*this)[1][0]));
    //painter->drawPoint((*this)[0][0], (*this)[1][0]);
}

Obj* Point::transform(Matrix m) {
    Matrix result = m * (*this);
    (*this)[0][0] = result[0][0];
    (*this)[1][0] = result[1][0];
    return this;
}
