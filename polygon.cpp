#include "polygon.h"
#include <QPainter>
#include <QVector>
#include <QDebug>

// Construtor: inicializa o objeto e copia a lista de vértices recebida
Polygon::Polygon(const QList<Point>& vertices, int id, QString name)
    : Obj(id, name, Type::Polygon), vertices(vertices) {
}

// Método para desenhar o polígono na tela
void Polygon::draw(QPainter *painter,
                   double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                   double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{
    if (vertices.size() > 1) {
        for (int i = 0; i < vertices.size() - 1; ++i) {
            // Ponto 1: Mundo -> NDC -> Viewport
            Point P1_ndc = vertices[i].normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x1 = Xvpmin + (P1_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y1 = Yvpmin + (1.0 - P1_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            // Ponto 2: Mundo -> NDC -> Viewport
            Point P2_ndc = vertices[i+1].normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x2 = Xvpmin + (P2_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y2 = Yvpmin + (1.0 - P2_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            painter->drawLine(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
        }
        // Se o polígono for fechado, desenha a linha do último ao primeiro ponto
        if(this->closed) {
            // Último ponto
            Point P_last_ndc = vertices.last().normalize(Xwmin, Ywmin, Xwmax, Ywmax);
            double x1 = Xvpmin + (P_last_ndc[0][0] + 1.0) / 2.0 * (Xvpmax - Xvpmin);
            double y1 = Yvpmin + (1.0 - P_last_ndc[1][0]) / 2.0 * (Yvpmax - Yvpmin);

            // Primeiro ponto
            Point P_first_ndc = vertices.first().normalize(Xwmin, Ywmin, Xwmax, Ywmax);
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
