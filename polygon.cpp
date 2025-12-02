#include "polygon.h"
#include <QPainter>
#include <QVector>
#include <QDebug>
#include "matrixmath.h"

const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;
// Construtor: inicializa o objeto e copia a lista de vértices recebida
Polygon::Polygon(const QList<Point>& vertices, int id, QString name)
    : Obj(id, name, Type::Polygon), vertices(vertices) {
}
Polygon::Polygon(const QList<Point>& vertices): Obj(), vertices(vertices)
{
}

// Método para desenhar o polígono na tela
void Polygon::draw(QPainter *painter, double dist, bool perspectflag, Matrix viewMatrix,
                   double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                   double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{
    if (vertices.size() > 1) {
        // Loop pelos vértices
        for (int i = 0; i < vertices.size() - 1; ++i) {
            Point P1 = vertices[i];
            Point P2 = vertices[i+1];

            Matrix camOffSet = viewMatrix * P1;
            P1[0][0] = camOffSet[0][0];
            P1[1][0] = camOffSet[1][0];
            P1[2][0] = camOffSet[2][0];

            camOffSet = viewMatrix * P2;
            P2[0][0] = camOffSet[0][0];
            P2[1][0] = camOffSet[1][0];
            P2[2][0] = camOffSet[2][0];

            if(perspectflag){
                if (dist == 0) dist = 1;

                double centerX = (Xwmin + Xwmax) / 2.0;
                double centerY = (Ywmin + Ywmax) / 2.0;

                Matrix p(4, 4);
                p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
                p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
                p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
                p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;

                Point auxP = P1;
                auxP[0][0] -= centerX;
                auxP[1][0] -= centerY;
                Matrix m = p * auxP;
                if (m[3][0] != 0) {
                    P1[0][0] = m[0][0] / m[3][0] + centerX;
                    P1[1][0] = m[1][0] / m[3][0] + centerY;
                    P1[2][0] = m[2][0] / m[3][0];
                }

                auxP = P2;
                auxP[0][0] -= centerX;
                auxP[1][0] -= centerY;
                m = p * auxP;
                if (m[3][0] != 0) {
                    P2[0][0] = m[0][0] / m[3][0] + centerX;
                    P2[1][0] = m[1][0] / m[3][0] + centerY;
                    P2[2][0] = m[2][0] / m[3][0];
                }
            }

            // Ponto 1: Mundo -> NDC -> Viewport
            double wx1 = P1[0][0];
            double wy1 = P1[1][0];
            double wx2 = P2[0][0];
            double wy2 = P2[1][0];


            bool visible = clippingCohen(wx1, wy1, wx2, wy2, Xwmin, Ywmin, Xwmax, Ywmax);
            if (!visible) {
                qDebug()<<"saiu da are poligono";
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

        // Fechamento do Polígono (Mesma lógica do loop acima)
        if(this->closed) {
            Point P_last = vertices.last();
            Point P_first = vertices.first();

            Matrix camOffSet = viewMatrix * P_last;
            P_last[0][0] = camOffSet[0][0];
            P_last[1][0] = camOffSet[1][0];
            P_last[2][0] = camOffSet[2][0];

            camOffSet = viewMatrix * P_first;
            P_first[0][0] = camOffSet[0][0];
            P_first[1][0] = camOffSet[1][0];
            P_first[2][0] = camOffSet[2][0];

            if(perspectflag){
                if (dist == 0) dist = 1;

                double centerX = (Xwmin + Xwmax) / 2.0;
                double centerY = (Ywmin + Ywmax) / 2.0;
                Matrix p(4, 4);
                p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
                p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
                p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
                p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;

                Point auxP = P_last;
                auxP[0][0] -= centerX;
                auxP[1][0] -= centerY;
                Matrix m = p * auxP;
                if (m[3][0] != 0) {
                    P_last[0][0] = m[0][0] / m[3][0] + centerX;
                    P_last[1][0] = m[1][0] / m[3][0] + centerY;
                    P_last[2][0] = m[2][0] / m[3][0];
                }

                auxP = P_first;
                auxP[0][0] -= centerX;
                auxP[1][0] -= centerY;
                m = p * auxP;
                if (m[3][0] != 0) {
                    P_first[0][0] = m[0][0] / m[3][0] + centerX;
                    P_first[1][0] = m[1][0] / m[3][0] + centerY;
                    P_first[2][0] = m[2][0] / m[3][0];
                }
            }

            double wx1 = P_last[0][0];
            double wy1 = P_last[1][0];
            double wx2 = P_first[0][0];
            double wy2 = P_first[1][0];
            // Viewport transform
            bool visible = clippingCohen(wx1, wy1, wx2, wy2, Xwmin, Ywmin, Xwmax, Ywmax);
            if (!visible) {
                qDebug()<<"saiu da are poligono";

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
    }
}
int Polygon::Regiao(double x, double y, double Xwmin, double Ywmin, double Xwmax, double Ywmax)
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


bool Polygon::clippingCohen(double& x1, double& y1, double& x2, double& y2, double Xwmin, double Ywmin, double Xwmax, double Ywmax)
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

// Método para aplicar uma transformação matricial a todos os vértices
Obj* Polygon::transform(Matrix m) {

    QList<Point> transformedVertices;
    for (const Point& p : vertices) {
        // Multiplica a matriz de transformação (m) por cada ponto (p)
        Matrix result = m * p;
        // Cria um novo ponto com o resultado da transformação
        transformedVertices.append(Point(result[0][0], result[1][0], result[2][0]));
    }
    // Atualiza os vértices do polígono com os novos pontos transformados
    this->vertices = transformedVertices;

    return this;
}

void Polygon::addPoint(Point p){
    this->vertices.append(p);
}
