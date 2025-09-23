#ifndef LINE_H
#define LINE_H

#include "object.h"
#include "point.h"
#include "matrix.h"

class Line : public Obj {
public:
    // Construtor que recebe os dois pontos que definem a linha
    Line(const Point& p1, const Point& p2, int id, QString name);

    // Métodos virtuais da classe Obj que precisam ser implementados

    void draw(QPainter *painter,
              double Xwmin, double Ywmin, double Xwmax, double Ywmax,
              double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax) override;
    Obj* transform(const Matrix m) override;

    Point getP1() const {return p1; }
    Point getP2() const {return p2; }

private:
    // Pontos inicial e final da linha
    Point p1;
    Point p2;
};

#endif // LINE_H
