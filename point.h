#ifndef POINT_H
#define POINT_H
#include <matrix.h>
#include <object.h>

class Point : public Obj, public Matrix
{
private:
    double x1;
    double y1;
public:
    Point(double x, double y, int id, QString name);
    Point(double x, double y);
    Point(Point *p);
    double getX() const;
    double getY()const;
    Point normalize(double Xwmin, double Ywmin, double Xwmax, double Ywmax);

    void draw(QPainter *painter,
              double Xwmin, double Ywmin, double Xwmax, double Ywmax,
              double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax) override;
    Obj* transform(Matrix m) override;
};

#endif // POINT_H
