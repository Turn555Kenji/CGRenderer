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

void Point::draw(QPainter *painter,
                 double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                 double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{

    QPen pen (Qt ::cyan, 5);
    pen.setCapStyle(Qt::RoundCap); // Crucial para o ponto parecer redondo
    painter->setPen(pen);

    // 1. Converte as coordenadas do Mundo para NDC [-1, 1]
    Point P_ndc = normalize(Xwmin, Ywmin, Xwmax, Ywmax);

    // 2. Converte as coordenadas NDC para o Viewport (tela)
    double screenX = Xvpmin + (P_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double screenY = Yvpmin + (1.0 - P_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin); // Eixo Y invertido

    painter->drawPoint(static_cast<int>(screenX), static_cast<int>(screenY));
}

Obj* Point::transform(Matrix m) {
    Matrix result = m * (*this);
    (*this)[0][0] = result[0][0];
    (*this)[1][0] = result[1][0];
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
