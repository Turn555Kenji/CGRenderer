#ifndef LINE_H
#define LINE_H
#include <matrix.h>
#include <object.h>
#include <point.h>

class Line : public Obj
{
public:
    Line(Point initP1, Point initP2, int id, const QString &name, const QString &type);

    void draw(QPainter *painter) override;
    Obj* transform(Matrix m) override;

    // Getters
    Point getP1() const { return p1; }
    Point getP2() const { return p2; }
private:
    Point p1;
    Point p2;
};

#endif // LINE_H
