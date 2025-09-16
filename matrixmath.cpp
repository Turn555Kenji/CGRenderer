#include "matrixmath.h"

MatrixMath::MatrixMath() {}

void MatrixMath::translateObject(Obj *target, int dx, int dy){
    Matrix t(3, 3);
    t[0][0] = 1; t[0][1] = 0; t[0][2] = dx;
    t[1][0] = 0; t[1][1] = 1; t[1][2] = dy;
    t[2][0] = 0; t[2][1] = 0; t[2][2] = 1;

    target->transform(t);
}

void MatrixMath::rotateObject(Obj *target, int angle){
    if (!target || target->getType() == "Point") {
        return;
    }

    Point pivot = getObjectPivot(target);

    double rad = angle * M_PI / 180.0;
    double c = std::cos(rad);
    double s = std::sin(rad);

    Matrix toOrigin(3, 3), rotation(3, 3), fromOrigin(3, 3);

    toOrigin[0][0] = 1; toOrigin[0][1] = 0; toOrigin[0][2] = -pivot[0][0];
    toOrigin[1][0] = 0; toOrigin[1][1] = 1; toOrigin[1][2] = -pivot[1][0];
    toOrigin[2][0] = 0; toOrigin[2][1] = 0; toOrigin[2][2] = 1;

    rotation[0][0] = c;  rotation[0][1] = -s; rotation[0][2] = 0;
    rotation[1][0] = s;  rotation[1][1] = c;  rotation[1][2] = 0;
    rotation[2][0] = 0;  rotation[2][1] = 0;  rotation[2][2] = 1;

    fromOrigin[0][0] = 1; fromOrigin[0][1] = 0; fromOrigin[0][2] = pivot[0][0];
    fromOrigin[1][0] = 0; fromOrigin[1][1] = 1; fromOrigin[1][2] = pivot[1][0];
    fromOrigin[2][0] = 0; fromOrigin[2][1] = 0; fromOrigin[2][2] = 1;

    Matrix compositeTransform = fromOrigin * rotation * toOrigin;

    target->transform(compositeTransform);
}

void MatrixMath::scaleObject(Obj *target, double sx, double sy){
    if (!target || target->getType() == "Point") {
        return;
    }

    Point pivot = getObjectPivot(target);

    Matrix toOrigin(3, 3), scale(3, 3), fromOrigin(3, 3);
    toOrigin[0][0] = 1; toOrigin[0][1] = 0; toOrigin[0][2] = -pivot[0][0];
    toOrigin[1][0] = 0; toOrigin[1][1] = 1; toOrigin[1][2] = -pivot[1][0];
    toOrigin[2][0] = 0; toOrigin[2][1] = 0; toOrigin[2][2] = 1;

    scale[0][0] = sx; scale[0][1] = 0;  scale[0][2] = 0;
    scale[1][0] = 0;  scale[1][1] = sy; scale[1][2] = 0;
    scale[2][0] = 0;  scale[2][1] = 0;  scale[2][2] = 1;

    fromOrigin[0][0] = 1; fromOrigin[0][1] = 0; fromOrigin[0][2] = pivot[0][0];
    fromOrigin[1][0] = 0; fromOrigin[1][1] = 1; fromOrigin[1][2] = pivot[1][0];
    fromOrigin[2][0] = 0; fromOrigin[2][1] = 0; fromOrigin[2][2] = 1;

    Matrix compositeTransform = fromOrigin * scale * toOrigin;

    target->transform(compositeTransform);
}

Point MatrixMath::getObjectPivot(Obj* obj) {
    if (!obj) return Point(0, 0);

    if (obj->getType() == "Point") {
        Point *pt = dynamic_cast<Point*>(obj);
        return *pt;
    } else if (obj->getType() == "Line") {
        Line *pt = dynamic_cast<Line*>(obj);
        return pt->getP1();
    } else if (obj->getType() == "Polygon") {
        Polygon *pt = dynamic_cast<Polygon*>(obj);
        return pt->getP1();
    }

    return Point(0, 0);
}
