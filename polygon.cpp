#include "polygon.h"
#include <QPainter>
#include <QVector>

// Construtor: inicializa o objeto e copia a lista de vértices recebida
Polygon::Polygon(const QList<Point>& vertices, int id, QString name)
    : Obj(id, name, Type::Polygon), vertices(vertices) {
}

// Método para desenhar o polígono na tela
void Polygon::draw(QPainter *painter) {
    if (vertices.size() > 1) {
        for (int i = 0; i < vertices.size() - 1; ++i) {
            // Extrai as coordenadas dos pontos consecutivos
            int x1 = static_cast<int>(vertices[i][0][0]);
            int y1 = static_cast<int>(vertices[i][1][0]);
            int x2 = static_cast<int>(vertices[i + 1][0][0]);
            int y2 = static_cast<int>(vertices[i + 1][1][0]);

            // Desenha a linha entre eles
            painter->drawLine(x1, y1, x2, y2);
        }
        // Se o polígono for fechado, desenha a linha do último ao primeiro ponto
        if(this->closed) {
            int x1 = static_cast<int>(vertices.last()[0][0]);
            int y1 = static_cast<int>(vertices.last()[1][0]);
            int x2 = static_cast<int>(vertices.first()[0][0]);
            int y2 = static_cast<int>(vertices.first()[1][0]);

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
