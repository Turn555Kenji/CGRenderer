#ifndef POINT_H
#define POINT_H
#include <matrix.h>
#include <object.h>

class Point : public Obj, public Matrix
{
public:
    Point(double x, double y, int id, QString name, QString type);
};

#endif // POINT_H
