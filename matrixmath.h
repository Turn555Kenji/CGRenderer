#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include "matrix.h"
#include "object.h"
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "typeobj.h"


class MatrixMath
{
public:
    MatrixMath();

    Matrix perspectiveMatrix(double d);

    static void translateObject(Obj *target, int dx, int dy, int dz);
    static void rotateObject(Obj *target, int angle, int axis, int pivotOption, int xpivot, int ypivot, int zpivot);
    static void scaleObject(Obj *target, double sx, double sy, double sz);
    static Point getObjectPivot(Obj* obj);
    static Point getObjectCenter(Obj* obj);
    static Matrix rotateWindow(double angleDegrees, double cx, double cy);

    static Matrix lookAt(Point eye, Point target, Point up);
    static Point subtract(Point a, Point b);
    static Point normalize(Point a);
    static double dotProduct(Point a, Point b);
    static Point crossProduct(Point a, Point b);
};

#endif // MATRIXMATH_H
