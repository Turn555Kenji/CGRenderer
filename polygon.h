#ifndef POLYGON_H
#define POLYGON_H

#include <QList>
#include "object.h"
#include "point.h"
#include "matrix.h"

class Polygon : public Obj {
public:
    Polygon(const QList<Point>& vertices, int id, QString name);

    void draw(QPainter *painter) override;

    //Obj* transform(const Matrix& m) override;

private:
    QList<Point> vertices;
};

#endif // POLYGON_H
