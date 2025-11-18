#include "line.h"
#include <QPainter>
#include "matrix.h" // <-- CORREÇÃO: ADICIONADO AQUI

// Construtor: inicializa o objeto e copia os pontos recebidos
Line::Line(const Point& p1, const Point& p2, int id, QString name)
    : Obj(id, name, Type::Line), p1(p1), p2(p2) {
    // A inicialização é feita na lista acima, o corpo pode ser vazio
}

// Método para desenhar a linha na tela
void Line::draw(QPainter *painter, double dist,
                double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{

    // Ponto 1: Mundo -> NDC -> Viewport
    Point P1_ndc = p1.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
    double x1 = Xvpmin + (P1_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double y1 = Yvpmin + (1.0 - P1_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

    // Ponto 2: Mundo -> NDC -> Viewport
    Point P2_ndc = p2.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
    double x2 = Xvpmin + (P2_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double y2 = Yvpmin + (1.0 - P2_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

    painter->drawLine(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
}

// Método para aplicar uma transformação matricial aos dois pontos da linha
Obj* Line::transform(Matrix m){
    Matrix result_p1 = m * p1;
    Matrix result_p2 = m * p2;

    this->p1 = Point(result_p1[0][0], result_p1[1][0], result_p1[2][0]);
    this->p2 = Point(result_p2[0][0], result_p2[1][0], result_p2[2][0]);

    return this;
}
