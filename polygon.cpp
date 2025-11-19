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
void Polygon::draw(QPainter *painter, double dist,
                   double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                   double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{
    if (vertices.size() > 1) {
        for (int i = 0; i < vertices.size() - 1; ++i) {
            // CORREÇÃO AQUI: Alterado de Matrix para Point
            Point P1 = vertices[i];
            Point P2 = vertices[i+1];

            // Matriz de projeção corrigida
            Matrix p(4, 4);
            p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
            p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
            p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
            p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;

            // Aplica projeção em P1
            Matrix m = p * P1;
            // Normalização Homogênea (Divide por W)
            if (m[3][0] != 0) {
                P1[0][0] = m[0][0] / m[3][0];
                P1[1][0] = m[1][0] / m[3][0];
                P1[2][0] = m[2][0] / m[3][0];
            }

            // Aplica projeção em P2
            m = p * P2;
            if (m[3][0] != 0) {
                P2[0][0] = m[0][0] / m[3][0];
                P2[1][0] = m[1][0] / m[3][0];
                P2[2][0] = m[2][0] / m[3][0];
            }

            // Ponto 1: Mundo -> NDC -> Viewport
            Point P1_ndc = P1.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x1 = Xvpmin + (P1_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y1 = Yvpmin + (1.0 - P1_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            // Ponto 2: Mundo -> NDC -> Viewport
            Point P2_ndc = P2.normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x2 = Xvpmin + (P2_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y2 = Yvpmin + (1.0 - P2_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            painter->drawLine(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
        }

        // Se o polígono for fechado, desenha a linha do último ao primeiro ponto
        if(this->closed) {
            // CORREÇÃO AQUI: Alterado de Matrix para Point
            Point P_last = vertices.last();
            Point P_first = vertices.first();

            Matrix p(4, 4);
            p[0][0] = 1; p[0][1] = 0; p[0][2] = 0; p[0][3] = 0;
            p[1][0] = 0; p[1][1] = 1; p[1][2] = 0; p[1][3] = 0;
            p[2][0] = 0; p[2][1] = 0; p[2][2] = 1; p[2][3] = 0;
            p[3][0] = 0; p[3][1] = 0; p[3][2] = 1/dist; p[3][3] = 1;

            // Projeção Último
            Matrix m = p * P_last;
            if (m[3][0] != 0) {
                P_last[0][0] = m[0][0] / m[3][0];
                P_last[1][0] = m[1][0] / m[3][0];
                P_last[2][0] = m[2][0] / m[3][0];
            }

            // Projeção Primeiro
            m = p * P_first;
            if (m[3][0] != 0) {
                P_first[0][0] = m[0][0] / m[3][0];
                P_first[1][0] = m[1][0] / m[3][0];
                P_first[2][0] = m[2][0] / m[3][0];
            }

            // Viewport transform
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
