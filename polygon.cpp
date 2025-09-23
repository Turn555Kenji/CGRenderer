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
            Point Pnorm1 = vertices[i].normalize(Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
            Point Pnorm2 = vertices[i+1].normalize(Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
            // Extrai as coordenadas dos pontos consecutivos
            int x1 = static_cast<int>(Pnorm1[0][0]);
            int y1 = static_cast<int>(Pnorm1[1][0]);
            int x2 = static_cast<int>(Pnorm2[0][0]);
            int y2 = static_cast<int>(Pnorm2[1][0]);
            //qDebug() << Pnorm1 << "NEW" << Pnorm2;

            // Desenha a linha entre eles
            painter->drawLine(x1, y1, x2, y2);
        }
        // Se o polígono for fechado, desenha a linha do último ao primeiro ponto
        if(this->closed) {
            Point PnormLast = vertices.last().normalize(Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
            Point PnormFirst = vertices.first().normalize(Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);

            int x1 = static_cast<int>(PnormLast[0][0]);
            int y1 = static_cast<int>(PnormLast[1][0]);
            int x2 = static_cast<int>(PnormFirst[0][0]);
            int y2 = static_cast<int>(PnormFirst[1][0]);
            //qDebug() << PnormLast << "FINAL" << PnormFirst;
            painter->drawLine(x1, y1, x2, y2);
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
        transformedVertices.append(Point(result[0][0], result[1][0]));
    }
    // Atualiza os vértices do polígono com os novos pontos transformados
    this->vertices = transformedVertices;

    return this;
}

void Polygon::addPoint(Point p){
    this->vertices.append(p);
}
