#ifndef POLYGON_H
#define POLYGON_H

#include <QList>
#include "object.h"
#include "point.h"
#include "matrix.h"

class Polygon : public Obj {
public:
    Polygon(const QList<Point>& vertices, int id, QString name);

    void draw(QPainter *painter,
              double Xwmin, double Ywmin, double Xwmax, double Ywmax,
              double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax) override;
    Obj* transform(Matrix m) override;

    void addPoint(Point p);
    void setClosed() { closed = true; }
    void setVertices(const QList<Point>& newVertices) { vertices = newVertices; } // <-- ADICIONAR ESTA LINHA

    QList<Point> getVertices() const { return vertices; }
    Point getP1() const { return vertices.first(); }

private:
    QList<Point> vertices;
    bool closed = false;
};

#endif // POLYGON_H
