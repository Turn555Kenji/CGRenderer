#include "line.h"
#include <QPainter>
#include "matrix.h"

// Construtor: inicializa o objeto e copia os pontos recebidos
Line::Line(const Point& p1, const Point& p2, int id, QString name)
    : Obj(id, name, Type::Line), p1(p1), p2(p2) {
}

// Método para desenhar a linha na tela
void Line::draw(QPainter *painter, double dist, bool perspectflag, Matrix viewMatrix,
                double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{
    // Copias locais para aplicar projeção sem alterar o objeto original
    Point P1_proj = p1;
    Point P2_proj = p2;

    Matrix camOffSet = viewMatrix * P1_proj;
    P1_proj[0][0] = camOffSet[0][0];
    P1_proj[1][0] = camOffSet[1][0];
    P1_proj[2][0] = camOffSet[2][0];

    camOffSet = viewMatrix * P2_proj;
    P2_proj[0][0] = camOffSet[0][0];
    P2_proj[1][0] = camOffSet[1][0];
    P2_proj[2][0] = camOffSet[2][0];

    if(perspectflag){
        // 1. Matriz de projeção
        Matrix p(4, 4);
        p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
        p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
        p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
        p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 0; // Use 0 aqui

        // 2. COMBINA AS MATRIZES (Projeção * Câmera)
        Matrix pv = p * viewMatrix;

        // 3. Aplica em P1 (pv * P1)
        Matrix m = pv * P1_proj;
        if (m[3][0] != 0) {
            P1_proj[0][0] = m[0][0] / m[3][0];
            P1_proj[1][0] = m[1][0] / m[3][0];
            P1_proj[2][0] = m[2][0] / m[3][0];
        }

        // 4. Aplica em P2 (pv * P2)
        m = pv * P2_proj;
        if (m[3][0] != 0) {
            P2_proj[0][0] = m[0][0] / m[3][0];
            P2_proj[1][0] = m[1][0] / m[3][0];
            P2_proj[2][0] = m[2][0] / m[3][0];
        }
    }

    // Viewport Transform
    Point P1_ndc = P1_proj.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
    double x1 = Xvpmin + (P1_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double y1 = Yvpmin + (1.0 - P1_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

    Point P2_ndc = P2_proj.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
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
