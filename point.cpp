#include "point.h"
#include <QString>
#include <QDebug>

Point::Point(double x, double y, double z, int id, QString name)
    : Obj(id, name, Type::Point), Matrix(4, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = z;
    (*this)[3][0] = 1;
}


Point::Point(double x, double y, int id, QString name)
    : Obj(id, name, Type::Point), Matrix(4, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 0;
    (*this)[3][0] = 1;
}

//Used by lines and polygons
Point::Point(double x, double y, double z)
    : Matrix(4, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = z;
    (*this)[3][0] = 1;
}

Point::Point(double x, double y)
    : Matrix(4, 1) {
    (*this)[0][0] = x;
    (*this)[1][0] = y;
    (*this)[2][0] = 0;
    (*this)[3][0] = 1;
}

Point::Point(Point *p )
    : Matrix(3, 1) {
    *this = p;
}

void Point::draw(QPainter *painter, double dist, bool perspectflag, Matrix viewMatrix,
                 double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                 double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{
    QPen pen (Qt ::cyan, 5);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    // Copia para não alterar o ponto original
    Point P_proj = *this;

    Matrix camOffSet = viewMatrix * P_proj;
    P_proj[0][0] = camOffSet[0][0];
    P_proj[1][0] = camOffSet[1][0];
    P_proj[2][0] = camOffSet[2][0];

    if(perspectflag){
        double centerX = (Xwmin + Xwmax) / 2.0;
        double centerY = (Ywmin + Ywmax) / 2.0;
        P_proj[0][0] -= centerX;
        P_proj[1][0] -= centerY;

        Matrix p(4, 4);
        p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
        p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
        p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
        p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;

        Matrix m = p * P_proj;

        if (m[3][0] != 0) {
            P_proj[0][0] = m[0][0] / m[3][0];
            P_proj[1][0] = m[1][0] / m[3][0];
            P_proj[2][0] = m[2][0] / m[3][0];
        }
    }

    // Viewport Transform
    Point P_ndc = P_proj.normalize(Xwmin, Ywmin, Xwmax, Ywmax);

    double screenX = Xvpmin + (P_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double screenY = Yvpmin + (1.0 - P_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

    painter->drawPoint(static_cast<int>(screenX), static_cast<int>(screenY));
}

Obj* Point::transform(Matrix m) {
    Matrix result = m * (*this);
    (*this)[0][0] = result[0][0];
    (*this)[1][0] = result[1][0];
    (*this)[2][0] = result[2][0];
    return this;
}

Point Point::normalize(double Xwmin, double Ywmin, double Xwmax, double Ywmax)
{
    // Mapeia as coordenadas do mundo para o intervalo [-1, 1] (NDC)
    double ndcX = -1.0 + 2.0 * ((*this)[0][0] - Xwmin) / (Xwmax - Xwmin);
    double ndcY = -1.0 + 2.0 * ((*this)[1][0] - Ywmin) / (Ywmax - Ywmin);

    return Point(ndcX, ndcY);
}

double Point :: getX()const{
    return x1;
}

double Point :: getY()const{
    return y1;
}

double Point :: getZ()const{
    return z1;
}
