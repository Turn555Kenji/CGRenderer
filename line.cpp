#include "line.h"
#include <QPainter>
#include "matrix.h"
#include <QDebug>
const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;
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
        if (dist == 0) dist = 1;

        double centerX = (Xwmin + Xwmax) / 2.0;
        double centerY = (Ywmin + Ywmax) / 2.0;

        Matrix p(4, 4);
        p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
        p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
        p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
        p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;


        Point auxP = P1_proj;
        auxP[0][0] -= centerX;
        auxP[1][0] -= centerY;
        Matrix m = p * auxP;
        if (m[3][0] != 0) {
            P1_proj[0][0] = m[0][0] / m[3][0] + centerX;
            P1_proj[1][0] = m[1][0] / m[3][0] + centerY;
            P1_proj[2][0] = m[2][0] / m[3][0];
        }

        auxP = P2_proj;
        auxP[0][0] -= centerX;
        auxP[1][0] -= centerY;
        m = p * auxP;
        if (m[3][0] != 0) {
            P2_proj[0][0] = m[0][0] / m[3][0] + centerX;
            P2_proj[1][0] = m[1][0] / m[3][0] + centerY;
            P2_proj[2][0] = m[2][0] / m[3][0];
        }
    }
    double wx1 = P1_proj[0][0];
    double wy1 = P1_proj[1][0];
    double wx2 = P2_proj[0][0];
    double wy2 = P2_proj[1][0];
    // Viewport transform
    bool visible = clippingCohen(wx1, wy1, wx2, wy2, Xwmin, Ywmin, Xwmax, Ywmax);
    if (!visible) {
        qDebug()<<"saiu da are";
        return;
    }

    double ndcX1 = -1.0 + 2.0 * (wx1 - Xwmin) / (Xwmax - Xwmin);
    double ndcY1 = -1.0 + 2.0 * (wy1 - Ywmin) / (Ywmax - Ywmin);

    double x1 = Xvpmin + (ndcX1 + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double y1 = Yvpmin + (1.0 - ndcY1) / 2.0 * (Yvpmax - Yvpmin);

    // Ponto 2: Mundo -> NDC -> Viewport
    double ndcX2 = -1.0 + 2.0 * (wx2 - Xwmin) / (Xwmax - Xwmin);
    double ndcY2 = -1.0 + 2.0 * (wy2 - Ywmin) / (Ywmax - Ywmin);

    double x2 = Xvpmin + (ndcX2 + 1.0) / 2.0 * (Xvpmax - Xvpmin);
    double y2 = Yvpmin + (1.0 - ndcY2) / 2.0 * (Yvpmax - Yvpmin);


    painter->drawLine(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
}
int Line::Regiao(double x, double y, double Xwmin, double Ywmin, double Xwmax, double Ywmax)
{
    int code = INSIDE;

    if (x < Xwmin)
        code |= LEFT;
    else if (x > Xwmax)
        code |= RIGHT;

    if (y < Ywmin)
        code |= BOTTOM;
    else if (y > Ywmax)
        code |= TOP;

    return code;
}


bool Line::clippingCohen(double& x1, double& y1, double& x2, double& y2, double Xwmin, double Ywmin, double Xwmax, double Ywmax)
{
    int code1 = Regiao(x1, y1, Xwmin, Ywmin,Xwmax,Ywmax);
    int code2 = Regiao(x2, y2, Xwmin, Ywmin,Xwmax,Ywmax);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) { //dentro da window
            accept = true;
            break;
        } else if (code1 & code2) {
            // Caso2 os dois estão fora
            accept = false;
            break;
        } else {
            // Caso 3 segmento de reta
            double x, y;
            int codeOut;
            // seleciona a parte da reta que está pra fora
            if (code1 != 0) {
                codeOut = code1;
            } else {
                codeOut = code2;
            }

            // interseção.
            if (codeOut & TOP) { //acima
                x = x1 + (x2 - x1) * (Ywmax - y1) / (y2 - y1);
                y = Ywmax;
            } else if (codeOut & BOTTOM) {//abaixo
                x = x1 + (x2 - x1) * (Ywmin - y1) / (y2 - y1);
                y = Ywmin;
            } else if (codeOut & RIGHT) {//direita
                y = y1 + (y2 - y1) * (Xwmax - x1) / (x2 - x1);
                x = Xwmax;
            } else if (codeOut & LEFT) {   //esquerda
                y = y1 + (y2 - y1) * (Xwmin - x1) / (x2 - x1);
                x = Xwmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = Regiao(x1, y1, Xwmin, Ywmin,Xwmax,Ywmax);
            } else {
                x2 = x;
                y2 = y;
                code2 = Regiao(x2, y2, Xwmin, Ywmin,Xwmax,Ywmax);
            }
        }
    }
    return accept;
}
// Método para aplicar uma transformação matricial aos dois pontos da linha
Obj* Line::transform(Matrix m){
    Matrix result_p1 = m * p1;
    Matrix result_p2 = m * p2;

    this->p1 = Point(result_p1[0][0], result_p1[1][0], result_p1[2][0]);
    this->p2 = Point(result_p2[0][0], result_p2[1][0], result_p2[2][0]);

    return this;
}
