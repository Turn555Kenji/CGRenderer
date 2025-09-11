#include "line.h"
#include <QPainter>
#include "matrix.h" // <-- CORREÇÃO: ADICIONADO AQUI

// Construtor: inicializa o objeto e copia os pontos recebidos
Line::Line(const Point& p1, const Point& p2, int id, QString name)
    : Obj(id, name, Type::Line), p1(p1), p2(p2) {
    // A inicialização é feita na lista acima, o corpo pode ser vazio
}

// Método para desenhar a linha na tela
void Line::draw(QPainter *painter) {
    int x1 = static_cast<int>(p1[0][0]);
    int y1 = static_cast<int>(p1[1][0]);

    int x2 = static_cast<int>(p2[0][0]);
    int y2 = static_cast<int>(p2[1][0]);

    painter->drawLine(x1, y1, x2, y2);
}

// Método para aplicar uma transformação matricial aos dois pontos da linha
/*Obj Line::transform(const Matrix& m) {
    /*Matrix result_p1 = m * p1;
    Matrix result_p2 = m * p2;

    this->p1 = Point(result_p1[0][0], result_p1[1][0]);
    this->p2 = Point(result_p2[0][0], result_p2[1][0]);

    return this;
}
*/
