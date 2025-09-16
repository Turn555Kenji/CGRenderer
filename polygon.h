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

    Obj* transform(Matrix m) override;

    void addPoint(Point p);

    QList<Point> getVertices() const { return vertices; }
    Point getP1() const { return vertices.first(); }

    void setClosed() { closed = true; }

private:
    QList<Point> vertices;
    bool closed = false;
};

#endif // POLYGON_H
