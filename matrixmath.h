#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include "matrix.h"
#include "object.h"
#include "point.h"
#include "line.h"
#include "polygon.h"


class MatrixMath
{
public:
    MatrixMath();

    static void translateObject(Obj *target, int dx, int dy, int dz);
    static void rotateObject(Obj *target, int angle, int xpivot, int ypivot);
    static void scaleObject(Obj *target, double sx, double sy);
    static Point getObjectPivot(Obj* obj);
    static Point getObjectCenter(Obj* obj);
    static Matrix rotateWindow(double angleDegrees, double cx, double cy);
};

#endif // MATRIXMATH_H
