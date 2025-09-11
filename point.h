#ifndef POINT_H
#define POINT_H
#include <matrix.h>
#include <object.h>

class Point : public Obj, public Matrix
{
public:
    Point(double x, double y, int id, QString name);
    Point(double x, double y);
    Point(Point *p);

    void draw(QPainter *painter) override;
    Obj* transform(Matrix m) override;

    double getY() const {return (*this)[0][0]; }
    double getX() const {return (*this)[1][0]; }



};

#endif // POINT_H
