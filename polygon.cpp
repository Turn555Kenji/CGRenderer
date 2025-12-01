#include "polygon.h"
#include <QPainter>
#include <QVector>
#include <QDebug>
#include "matrixmath.h"

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
                // 1. Configura a Projeção
                Matrix p(4, 4);
                p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
                p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
                p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
                p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 0; // Use 0 aqui para corrigir a distorção

                // 2. A MÁGICA ACONTECE AQUI: Combina Projeção com Câmera
                Matrix pv = p * viewMatrix;

                // 3. Aplica no Ponto 1 (sem variáveis auxiliares)
                Matrix m = pv * P1;
                if (m[3][0] != 0) {
                    P1[0][0] = m[0][0] / m[3][0];
                    P1[1][0] = m[1][0] / m[3][0];
                    P1[2][0] = m[2][0] / m[3][0];
                }

                // 4. Aplica no Ponto 2
                m = pv * P2;
                if (m[3][0] != 0) {
                    P2[0][0] = m[0][0] / m[3][0];
                    P2[1][0] = m[1][0] / m[3][0];
                    P2[2][0] = m[2][0] / m[3][0];
                }
            }

            // Desenha na tela (Viewport)
            Point P1_ndc = P1.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x1 = Xvpmin + (P1_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y1 = Yvpmin + (1.0 - P1_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            Point P2_ndc = P2.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x2 = Xvpmin + (P2_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y2 = Yvpmin + (1.0 - P2_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

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
                Matrix p(4, 4);
                p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
                p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
                p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
                p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 0;

                Matrix pv = p * viewMatrix; // <--- MUITO IMPORTANTE

                Matrix m = pv * P_last;
                if (m[3][0] != 0) {
                    P_last[0][0] = m[0][0] / m[3][0];
                    P_last[1][0] = m[1][0] / m[3][0];
                    P_last[2][0] = m[2][0] / m[3][0];
                }

                m = pv * P_first;
                if (m[3][0] != 0) {
                    P_first[0][0] = m[0][0] / m[3][0];
                    P_first[1][0] = m[1][0] / m[3][0];
                    P_first[2][0] = m[2][0] / m[3][0];
                }
            }

            Point P_last_ndc = P_last.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x1 = Xvpmin + (P_last_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y1 = Yvpmin + (1.0 - P_last_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            Point P_first_ndc = P_first.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x2 = Xvpmin + (P_first_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y2 = Yvpmin + (1.0 - P_first_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            painter->drawLine(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
        }
    }
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
