#ifndef POLYGON_H
#define POLYGON_H

#include <QList>
#include "object.h"
#include "point.h"
#include "matrix.h"

class Polygon : public Obj {
public:
    Polygon(const QList<Point>& vertices, int id, QString name);
    Polygon(const QList<Point>& vertices);


    void draw(QPainter *painter, double dist, bool perspectflag, Matrix viewMatrix,
              double Xwmin, double Ywmin, double Xwmax, double Ywmax,
              double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax) override;
    Obj* transform(Matrix m) override;

    void addPoint(Point p);
    void setClosed() { closed = true; }
    void setVertices(const QList<Point>& newVertices) { vertices = newVertices; } // <-- ADICIONAR ESTA LINHA

    QList<Point> getVertices() const { return vertices; }
    Point getP1() const { return vertices.first(); }

private:
    int Regiao(double x, double y, double Xwmin, double Ywmin, double Xwmax, double Ywmax);
    bool clippingCohen(double& x1, double& y1, double& x2, double& y2, double Xwmin, double Ywmin, double Xwmax, double Ywmax);
    QList<Point> vertices;
    bool closed = false;
};

#endif // POLYGON_H
