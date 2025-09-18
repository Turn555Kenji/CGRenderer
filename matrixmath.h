#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include "matrix.h"
#include "object.h"
#include "point.h"
#include "line.h"
#include "polygon.h"
#include <cmath>

class MatrixMath
{
public:
    MatrixMath();

    static void translateObject(Obj *target, int dx, int dy);
    static void rotateObject(Obj *target, int angle, int xpivot, int ypivot);
    static void scaleObject(Obj *target, double sx, double sy);
    static Point getObjectPivot(Obj* obj);
};

#endif // MATRIXMATH_H
