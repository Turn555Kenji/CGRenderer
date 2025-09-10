#include "line.h"

Line::Line(Point initP1, Point initP2, int id, const QString &name, Type type)
    : Obj(id, name, type), p1(initP1), p2(initP2) {
}

void Line::draw(QPainter *painter) {

}

Obj* Line::transform(Matrix m) {

}
