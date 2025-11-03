#include "matrixmath.h"
#include <cmath>
#include <QDebug>
#define DEG2RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)
MatrixMath::MatrixMath() {}

void MatrixMath::translateObject(Obj *target, int dx, int dy, int dz){
    Matrix t(4, 4);
    t[0][0] = 1; t[0][1] = 0; t[0][2] = 0; t[0][3] = dx;
    t[1][0] = 0; t[1][1] = 1; t[1][2] = 0; t[1][3] = dy;
    t[2][0] = 0; t[2][1] = 0; t[2][2] = 1; t[2][3] = dz;
    t[3][0] = 0; t[3][1] = 0; t[3][2] = 0; t[3][3] = 1;

    target->transform(t);
}

void MatrixMath::rotateObject(Obj *target, int angle, int xpivot, int ypivot){
    if (!target || target->getType() == "Point") {
        return;
    }

    Point pivot = Point(xpivot, ypivot);
    qDebug() << pivot;

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

void MatrixMath::scaleObject(Obj *target, double sx, double sy, double sz){
    if (!target || target->getType() == "Point") {
        return;
    }

    Point pivot = getObjectCenter(target);

    Matrix toOrigin(4, 4), scale(4, 4), fromOrigin(4, 4);
    toOrigin[0][0] = 1; toOrigin[0][1] = 0; toOrigin[0][2] = 0; toOrigin[0][3] = -pivot[0][3];
    toOrigin[1][0] = 0; toOrigin[1][1] = 1; toOrigin[1][2] = 0; toOrigin[1][3] = -pivot[1][3];
    toOrigin[2][0] = 0; toOrigin[2][1] = 0; toOrigin[2][2] = 1; toOrigin[2][3] = -pivot[2][3];
    toOrigin[3][0] = 0; toOrigin[3][1] = 0; toOrigin[3][2] = 0; toOrigin[3][3] = 1;

    scale[0][0] = sx; scale[0][1] = 0;  scale[0][2] = 0;  scale[0][3] = 0;
    scale[1][0] = 0;  scale[1][1] = sy; scale[1][2] = 0;  scale[1][3] = 0;
    scale[2][0] = 0;  scale[2][1] = 0;  scale[2][2] = sz; scale[2][3] = 0;
    scale[3][0] = 0;  scale[3][1] = 0;  scale[3][2] = 0;  scale[3][3] = 1;

    fromOrigin[0][0] = 1; fromOrigin[0][1] = 0; fromOrigin[0][2] = 0; fromOrigin[0][3] = pivot[0][3];
    fromOrigin[1][0] = 0; fromOrigin[1][1] = 1; fromOrigin[1][2] = 0; fromOrigin[1][3] = pivot[1][3];
    fromOrigin[2][0] = 0; fromOrigin[2][1] = 0; fromOrigin[2][2] = 1; fromOrigin[2][3] = pivot[2][3];
    fromOrigin[3][0] = 0; fromOrigin[3][1] = 0; fromOrigin[3][2] = 0; fromOrigin[3][3] = 1;

    Matrix compositeTransform = fromOrigin * scale * toOrigin;

    target->transform(compositeTransform);
}

Point MatrixMath::getObjectPivot(Obj* obj) {    //Legacy code
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

Point MatrixMath::getObjectCenter(Obj* obj) {
    if (!obj) return Point(0, 0);

    if (obj->getType() == "Point") {
        return *(dynamic_cast<Point*>(obj));
    }
    else if (obj->getType() == "Line") {
        Line* ln = dynamic_cast<Line*>(obj);
        Point p1 = ln->getP1();
        Point p2 = ln->getP2();
        return Point((p1[0][0] + p2[0][0]) / 2.0, (p1[1][0] + p2[1][0]) / 2.0, (p1[2][0] + p2[2][0]) / 2.0);
    }
    else if (obj->getType() == "Polygon") {
        Polygon* poly = dynamic_cast<Polygon*>(obj);
        const QList<Point>& pts = poly->getVertices();

        if (pts.empty()) return Point(0, 0);

        double cx = 0, cy = 0, cz = 0;
        for (const auto& p : pts) {
            cx += p[0][0];
            cy += p[1][0];
            cz += p[2][0];
        }
        cx /= pts.size();
        cy /= pts.size();
        cz /= pts.size();

        return Point(cx, cy, cz); // <-- centroid (average of vertices)
    }

    return Point(0, 0);
}
